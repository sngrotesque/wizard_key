#include <crypto/WukOP4.hh>
#include <WukBinascii.hh>
#include <WukRandom.hh>
#include <WukTime.hh>
#include <WukMisc.hh>

#include <filesystem>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <thread>
#include <vector>
#include <future>

#include <openssl/evp.h>
#include <openssl/rand.h>

using namespace wuk::crypto;
using namespace wuk::misc;

namespace fs = std::filesystem;

constexpr wU32 OP4_SALT_LEN  = WukOP4_BL;
constexpr wU32 OP4_NONCE_LEN = WukOP4_NL;
constexpr wU32 PBKDF2_ROUNDS = 114514;
constexpr wU32 block_size    = 4096;

constexpr wU32 decryption_error = 777777777;

#define SPEED_TEST(func) \
    func; \
    double start = timer.time(); \
    func; \
    double stop = timer.time(); \
    double taken_time = stop - start; \
    double throughput = length / taken_time / (1024 * 1024); \
    printf("Token time: %.4lf\n", taken_time); \
    printf("Speed: %.2lf MB/s.\n", throughput);

#ifdef TEST
void weak_key_test()
{
    wByte key_l[WukOP4_KL] = {
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    wByte key_r[WukOP4_KL] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    WukOP4 op4_l(key_l);
    WukOP4 op4_r(key_r);

    const wByte *kl = op4_l.get_roundkey();
    const wByte *kr = op4_r.get_roundkey();

    std::cout << "Round key (left):\t\t\t\t\t\tRound key (Right):\n";
    print_diff_hex(kl, kr, WukOP4_RKL, WukOP4_RKL, WukOP4_BL, true);

    if (memcmp(kl, kr, WukOP4_RKL) == 0) {
        std::cout << "Weak key has been formed!\n";
        exit(decryption_error);
    } else {
        std::cout << "The key extension algorithm is secure.\n\n";
    }
}

void xcryption_verification()
{
    wByte key[WukOP4_KL]{
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F
    };
    wByte iv[WukOP4_BL]{
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF
    };
    wByte nonce[WukOP4_NL]{
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF
    };

    constexpr size_t length = WukOP4_BL << 1;
    wByte plaintext[length]{
        0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01,
        0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01,
        0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01,
        0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01
    };
    wByte ciphertext[length]{0};
    wByte decrypted[length]{0};
    WukOP4 op4(key);

    std::cout << "Master key:\n";
    print_hex(key, WukOP4_KL, WukOP4_BL, true, true);

    std::cout << "Master Nonce:\n";
    print_hex(nonce, WukOP4_NL, WukOP4_BL, true, true);

    std::cout << "Master IV:\n";
    print_hex(iv, WukOP4_BL, WukOP4_BL, true, true);

    std::cout << "Round key\n";
    print_hex(op4.get_roundkey(), WukOP4_RKL, WukOP4_BL, true, true);

    std::cout << "Plaintext\n";
    print_hex(plaintext, length, WukOP4_BL, true, true);

    op4.ecb_encrypt(ciphertext, plaintext, length);
    std::cout << "\x1b[92m" << "ECB Ciphertext\n" << "\x1b[0m";
    print_hex(ciphertext, length, WukOP4_BL, true, true);

    op4.ecb_decrypt(decrypted, ciphertext, length);
    if (memcmp(plaintext, decrypted, length) != 0) {
        std::cout << "\x1b[91m" << "[!] ECB Decryption failed! [!]\n" << "\x1b[0m";
        exit(decryption_error);
    }

    op4.cbc_encrypt(ciphertext, plaintext, length, iv);
    std::cout << "\x1b[94m" << "CBC Ciphertext\n" << "\x1b[0m";
    print_hex(ciphertext, length, WukOP4_BL, true, true);

    op4.cbc_decrypt(decrypted, ciphertext, length, iv);
    if (memcmp(plaintext, decrypted, length) != 0) {
        std::cout << "\x1b[91m" << "[!] CBC Decryption failed! [!]\n" << "\x1b[0m";
        exit(decryption_error);
    }

    op4.ofb_stream(ciphertext, plaintext, length, iv);
    std::cout << "\x1b[95m" << "OFB Ciphertext\n" << "\x1b[0m";
    print_hex(ciphertext, length, WukOP4_BL, true, true);

    op4.ofb_stream(decrypted, ciphertext, length, iv);
    if (memcmp(plaintext, decrypted, length) != 0) {
        std::cout << "\x1b[91m" << "[!] OFB Decryption failed! [!]\n" << "\x1b[0m";
        exit(decryption_error);
    }

    op4.ctr_stream(ciphertext, plaintext, length, nonce);
    std::cout << "\x1b[96m" << "CTR Ciphertext\n" << "\x1b[0m";
    print_hex(ciphertext, length, WukOP4_BL, true, true);

    op4.set_counter(0);
    op4.ctr_stream(decrypted, ciphertext, length, nonce);
    if (memcmp(plaintext, decrypted, length) != 0) {
        std::cout << "\x1b[91m" << "[!] CTR Decryption failed! [!]\n" << "\x1b[0m";
        exit(decryption_error);
    }
}
#endif

// 计算 SHA-256 哈希
static std::string hash_sha256(const void* data, size_t length) {
    wuk::Binascii ba;
    wByte digest[32]{0};

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
    EVP_DigestUpdate(ctx, data, length);
    EVP_DigestFinal_ex(ctx, digest, nullptr);
    EVP_MD_CTX_free(ctx);

    return ba.b2a_hex(std::string{(char *)digest, sizeof(digest)});
}

void derive_key_pbkdf2(const char *password, const wByte salt[OP4_SALT_LEN], wByte out_key[WukOP4_KL])
{
    PKCS5_PBKDF2_HMAC(password, strlen(password),
                      salt, OP4_SALT_LEN,
                      PBKDF2_ROUNDS,
                      EVP_sha256(),
                      WukOP4_KL, out_key);
}

void file_encrypt(fs::path input_file, fs::path output_file, const char *password)
{
    wByte salt[OP4_SALT_LEN];
    wByte nonce[OP4_NONCE_LEN];
    wByte key[WukOP4_KL];

    RAND_bytes(salt, sizeof salt);
    RAND_bytes(nonce, sizeof nonce);
    derive_key_pbkdf2(password, salt, key);

    std::ifstream fin(input_file, std::ios::binary);
    std::ofstream fout(output_file, std::ios::binary);
    if (!fin || !fout) throw std::runtime_error("file open failed");

    // 写入 salt 和 nonce 到输出文件头部
    fout.write((char*)salt, OP4_SALT_LEN);
    fout.write((char*)nonce, OP4_NONCE_LEN);

    WukOP4 op4(key);

    printf("Round key:\n");
    print_hex(op4.get_roundkey(), WukOP4_RKL, 16, true, true);

    wByte plaintext[block_size]{};
    wByte ciphertext[block_size]{};
    while (fin.read((char*)plaintext, block_size) || fin.gcount()) {
        size_t n = fin.gcount();
        op4.ctr_stream(ciphertext, plaintext, n, nonce);
        fout.write((char*)ciphertext, n);
    }
}

void file_decrypt(fs::path input_file, fs::path output_file, const char *password)
{
    std::ifstream fin(input_file, std::ios::binary);
    std::ofstream fout(output_file, std::ios::binary);
    if (!fin || !fout) throw std::runtime_error("file open failed");

    wByte salt[OP4_SALT_LEN];
    wByte nonce[OP4_NONCE_LEN];
    wByte key[WukOP4_KL];

    // 从加密文件头部读取 salt 和 nonce
    fin.read((char*)salt, OP4_SALT_LEN);
    fin.read((char*)nonce, OP4_NONCE_LEN);
    derive_key_pbkdf2(password, salt, key);

    WukOP4 op4(key);
    wByte ciphertext[block_size]{};
    wByte plaintext[block_size]{};
    while (fin.read((char*)ciphertext, block_size) || fin.gcount()) {
        size_t n = fin.gcount();
        op4.ctr_stream(plaintext, ciphertext, n, nonce);
        fout.write((char*)plaintext, n);
    }
}

#if defined(THREADS_METHOD) && (THREADS_METHOD == 1)
void op4_thread(wByte *ciphertext, const wByte *plaintext, wSize length,
                const wByte key[WukOP4_KL], const wByte nonce[WukOP4_NL],
                wU32 thread_count = 4)
{
    std::vector<std::thread> threads;
    wSize chunk_size = (length / thread_count + WukOP4_BL - 1) / WukOP4_BL * WukOP4_BL; // 对齐到块大小
    wSize remaining = length;

    for (wU32 i = 0; i < thread_count && remaining > 0; ++i) {
        wSize current_chunk = std::min(chunk_size, remaining);
        wSize aligned_chunk = current_chunk - (current_chunk % WukOP4_BL); // 完整块部分
        wSize extra_bytes = current_chunk % WukOP4_BL;                     // 尾部不完整块

        wU32 counter = (i * chunk_size) / WukOP4_BL; // 精确计算counter

        threads.emplace_back([=]() {
            WukOP4 op4{key, counter};
            // 处理完整块
            if (aligned_chunk > 0) {
                op4.ctr_stream(ciphertext + i * chunk_size, 
                               plaintext + i * chunk_size, 
                               aligned_chunk, nonce);
            }
            // 处理尾部不完整块（仅最后一个线程可能执行）
            if (extra_bytes > 0 && i == thread_count - 1) {
                wByte last_block[WukOP4_BL]{0};
                memcpy(last_block, plaintext + i * chunk_size + aligned_chunk, extra_bytes);
                op4.ctr_stream(last_block, last_block, WukOP4_BL, nonce); // 加密完整块
                memcpy(ciphertext + i * chunk_size + aligned_chunk, last_block, extra_bytes); // 只拷贝所需部分
            }
        });

        remaining -= current_chunk;
    }
    for (auto &t : threads) {
        t.join();
    }
}

#elif defined(THREADS_METHOD) && (THREADS_METHOD == 2)
void op4_thread(wByte *ciphertext, const wByte *plaintext, wSize length,
                         const wByte key[WukOP4_KL], const wByte nonce[WukOP4_NL],
                         wU32 thread_count) {
    constexpr wSize block_size = WukOP4_BL;
    wSize chunk_size = ((length / thread_count + block_size - 1) / block_size) * block_size;
    std::vector<std::future<void>> futures;

    for (wU32 i = 0; i < thread_count; ++i) {
        wSize offset = i * chunk_size;
        wSize size = (i == thread_count - 1) ? (length - offset) : chunk_size;
        wU32 counter = offset / block_size;

        futures.emplace_back(std::async(std::launch::async, [=]() {
            WukOP4 op4{key, counter};
            op4.ctr_stream(ciphertext + offset, plaintext + offset, size - (size % block_size), nonce);
            // 处理尾部不完整块（仅最后一个线程）
            if (i == thread_count - 1 && size % block_size != 0) {
                wByte last_block[block_size]{0};
                memcpy(last_block, plaintext + offset + size - (size % block_size), size % block_size);
                op4.ctr_stream(last_block, last_block, block_size, nonce);
                memcpy(ciphertext + offset + size - (size % block_size), last_block, size % block_size);
            }
        }));
    }
    for (auto &f : futures) f.wait();
}

#elif defined(THREADS_METHOD) && (THREADS_METHOD == 3)
struct ThreadArgs {
    wByte* ciphertext;
    const wByte* plaintext;
    wSize offset;
    wSize size;
    const wByte* key;
    const wByte* nonce;
    wU32 counter;
};

void op4_thread_worker(const ThreadArgs* args) {
    WukOP4 op4(args->key, args->counter);
    
    // 处理完整块
    wSize aligned_size = args->size - (args->size % WukOP4_BL);
    if (aligned_size > 0) {
        op4.ctr_stream(
            args->ciphertext + args->offset,
            args->plaintext + args->offset,
            aligned_size,
            args->nonce
        );
    }

    // 处理尾部不完整块（仅最后一个线程需要）
    if (args->size % WukOP4_BL != 0) {
        wByte last_block[WukOP4_BL] = {0};
        memcpy(last_block, 
              args->plaintext + args->offset + aligned_size,
              args->size % WukOP4_BL);
        
        op4.ctr_stream(last_block, last_block, WukOP4_BL, args->nonce);
        
        memcpy(args->ciphertext + args->offset + aligned_size,
              last_block,
              args->size % WukOP4_BL);
    }
}

void op4_thread(wByte* ciphertext, const wByte* plaintext, wSize length,
            const wByte key[WukOP4_KL], const wByte nonce[WukOP4_NL],
            wU32 thread_count = 4) 
{
    // 1. 计算分块参数
    const wSize block_size = WukOP4_BL;
    wSize chunk_size = ((length / thread_count + block_size - 1) / block_size) * block_size;
    std::vector<std::thread> threads;
    std::vector<ThreadArgs> args_list(thread_count);

    // 2. 准备线程参数
    wSize remaining = length;
    for (wU32 i = 0; i < thread_count && remaining > 0; ++i) {
        ThreadArgs args;
        args.ciphertext = ciphertext;
        args.plaintext = plaintext;
        args.offset = i * chunk_size;
        args.size = std::min(chunk_size, remaining);
        args.key = key;
        args.nonce = nonce;
        args.counter = args.offset / block_size;
        
        args_list[i] = args;
        remaining -= args.size;
    }

    // 3. 启动线程
    for (wU32 i = 0; i < thread_count; ++i) {
        if (args_list[i].size > 0) {
            threads.emplace_back(op4_thread_worker, &args_list[i]);
        }
    }

    // 4. 等待线程完成
    for (auto& t : threads) {
        t.join();
    }
}
#endif

void op4_single_thread(wByte* ciphertext, const wByte* plaintext, wSize length,
            const wByte key[WukOP4_KL], const wByte nonce[WukOP4_NL])
{
    // 如果不写在函数内部进行初始化的话，计数器会一直更新，导致多线程与单线程加密结果不一致、
    WukOP4 op4(key, 0);
    op4.ctr_stream(ciphertext, plaintext, length, nonce);
}

void op4_threads()
{
    // 128KB（131072 Bytes）是多线程弱于单线程性能的分水岭
    constexpr wSize length = static_cast<wSize>(128ULL * 1024*1024);
    wByte *plaintext = new (std::align_val_t(16), std::nothrow) wByte[length];
    if (!plaintext) {
        throw wuk::Exception(wuk::Error::MEMORY, "op4_threads",
            "failed to allocate for plaintext.");
    }
    wByte *ciphertext = new (std::align_val_t(16), std::nothrow) wByte[length];
    if (!ciphertext) {
        operator delete[](plaintext, std::align_val_t(16));
        throw wuk::Exception(wuk::Error::MEMORY, "op4_threads",
            "failed to allocate for ciphertext.");
    }
    wuk::memory_secure(plaintext, length);
    wuk::memory_secure(ciphertext, length);
    const wByte key[WukOP4_KL]{0};
    const wByte nonce[WukOP4_KL]{0};
    wuk::Time timer;

    std::cout << "The length of the encrypted data is: "
              << std::fixed << std::setprecision(2)
              << (static_cast<double>(length) / (1024*1024))
              << " MB." << std::endl;
#   if defined(THREADS_METHOD) && ((THREADS_METHOD >= 1) && (THREADS_METHOD <= 3))
    std::cout << "Multi threaded encryption is in progress.." << std::endl;
    wU32 thread_count = std::min(static_cast<wU32>(std::thread::hardware_concurrency()),
                                static_cast<wU32>((length + block_size - 1) / block_size));
    std::cout << "threads count: " << thread_count << std::endl;
    SPEED_TEST(op4_thread(ciphertext, plaintext, length, key, nonce, thread_count));
#   else
    std::cout << "single threaded encryption is in progress.." << std::endl;
    SPEED_TEST(op4_single_thread(ciphertext, plaintext, length, key, nonce));
#   endif
    std::cout << "Plaintext  hexdigest: " << hash_sha256(plaintext,  length) << std::endl;
    std::cout << "Ciphertext hexdigest: " << hash_sha256(ciphertext, length) << std::endl;

    wByte *decrypted = new (std::align_val_t(16), std::nothrow) wByte[length];
    if(!decrypted) {
        operator delete[](ciphertext, std::align_val_t(16));
        operator delete[](plaintext, std::align_val_t(16));
        throw wuk::Exception(wuk::Error::MEMORY, "op4_threads", "failed to allocate for ciphertext.");
    }
    op4_single_thread(decrypted, ciphertext, length, key, nonce);
    if(memcmp(plaintext, decrypted, length) != 0) {
        std::cout << "\x1b[91m[!] \x1b[0m"
                  << "Data decryption failed due to inconsistent code logic, unable to pass." << std::endl;
    }

    operator delete[](decrypted, std::align_val_t(16));
    operator delete[](ciphertext, std::align_val_t(16));
    operator delete[](plaintext, std::align_val_t(16));
}

void anonymous_function()
{
    auto f = [](int a) -> int {return a * 2;};
    std::cout << f(5) << std::endl;
}

int main()
{
#   ifdef TEST
    xcryption_verification();
#   endif
    op4_threads();

    return 0;
}
