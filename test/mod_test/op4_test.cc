#include <crypto/WukOP4.hh>
#include <crypto/WukHash.hh>
#include <WukBuffer.hh>
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

using namespace wuk::crypto;
using namespace wuk::misc;

#define SPEED_TEST(func) \
    func; \
    wuk::f64 start = timer.time<wuk::f64>(); \
    func; \
    wuk::f64 stop = timer.time<wuk::f64>(); \
    wuk::f64 taken_time = stop - start; \
    wuk::f64 throughput = length / taken_time / (1024 * 1024); \
    printf("Token time: %.4lf\n", taken_time); \
    printf("Speed: %.2lf MB/s.\n", throughput);
constexpr wuk::u32 block_size = 4096;

std::string hash_sha256(const wuk::byte *buffer, wuk::ulong length)
{
    Hashlib<HashlibType::SHA_256> hash;
    hash.update(buffer, length);
    return hash.hexdigest();
}

#ifdef TEST
namespace fs = std::filesystem;

constexpr wuk::u32 OP4_SALT_LEN  = OP4_BL;
constexpr wuk::u32 OP4_NONCE_LEN = OP4_NL;
constexpr wuk::u32 PBKDF2_ROUNDS = 114514;

constexpr wuk::u32 decryption_error = 777777777;

void weak_key_test()
{
    wuk::byte key_l[OP4_KL] = {
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    wuk::byte key_r[OP4_KL] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    OP4 op4_l(key_l);
    OP4 op4_r(key_r);

    const wuk::byte *kl = op4_l.get_roundkey();
    const wuk::byte *kr = op4_r.get_roundkey();

    std::cout << "Round key (left):\t\t\t\t\t\tRound key (Right):\n";
    print_diff_hex(kl, kr, OP4_RKL, OP4_RKL, OP4_BL, true);

    if (memcmp(kl, kr, OP4_RKL) == 0) {
        std::cout << "Weak key has been formed!\n";
        exit(decryption_error);
    } else {
        std::cout << "The key extension algorithm is secure.\n\n";
    }
}

void xcryption_verification()
{
    wuk::byte key[OP4_KL]{
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    wuk::byte iv[OP4_BL]{
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };
    wuk::byte nonce[OP4_NL]{
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };

    constexpr size_t length = OP4_BL << 1;
    wuk::byte plaintext[length]{
        0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01,
        0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01,
        0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01,
        0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01
    };
    wuk::byte ciphertext[length]{0};
    wuk::byte decrypted[length]{0};
    OP4 op4(key);

    std::cout << "Master key:\n";
    print_hex(key, OP4_KL, OP4_BL, true, true);

    std::cout << "Master Nonce:\n";
    print_hex(nonce, OP4_NL, OP4_BL, true, true);

    std::cout << "Master IV:\n";
    print_hex(iv, OP4_BL, OP4_BL, true, true);

    std::cout << "Round key\n";
    print_hex(op4.get_roundkey(), OP4_RKL, OP4_BL, true, true);

    std::cout << "Plaintext\n";
    print_hex(plaintext, length, OP4_BL, true, true);

    op4.ecb_encrypt(ciphertext, plaintext, length);
    std::cout << "\x1b[92m" << "ECB Ciphertext\n" << "\x1b[0m";
    print_hex(ciphertext, length, OP4_BL, true, true);

    op4.ecb_decrypt(decrypted, ciphertext, length);
    if (memcmp(plaintext, decrypted, length) != 0) {
        std::cout << "\x1b[91m" << "[!] ECB Decryption failed! [!]\n" << "\x1b[0m";
        exit(decryption_error);
    }

    op4.cbc_encrypt(ciphertext, plaintext, length, iv);
    std::cout << "\x1b[94m" << "CBC Ciphertext\n" << "\x1b[0m";
    print_hex(ciphertext, length, OP4_BL, true, true);

    op4.cbc_decrypt(decrypted, ciphertext, length, iv);
    if (memcmp(plaintext, decrypted, length) != 0) {
        std::cout << "\x1b[91m" << "[!] CBC Decryption failed! [!]\n" << "\x1b[0m";
        exit(decryption_error);
    }

    op4.ofb_stream(ciphertext, plaintext, length, iv);
    std::cout << "\x1b[95m" << "OFB Ciphertext\n" << "\x1b[0m";
    print_hex(ciphertext, length, OP4_BL, true, true);

    op4.ofb_stream(decrypted, ciphertext, length, iv);
    if (memcmp(plaintext, decrypted, length) != 0) {
        std::cout << "\x1b[91m" << "[!] OFB Decryption failed! [!]\n" << "\x1b[0m";
        exit(decryption_error);
    }

    op4.ctr_stream(ciphertext, plaintext, length, nonce);
    std::cout << "\x1b[96m" << "CTR Ciphertext\n" << "\x1b[0m";
    print_hex(ciphertext, length, OP4_BL, true, true);

    op4.set_counter(0);
    op4.ctr_stream(decrypted, ciphertext, length, nonce);
    if (memcmp(plaintext, decrypted, length) != 0) {
        std::cout << "\x1b[91m" << "[!] CTR Decryption failed! [!]\n" << "\x1b[0m";
        exit(decryption_error);
    }
}

void derive_key_pbkdf2(const char *password, const wuk::byte salt[OP4_SALT_LEN], wuk::byte out_key[OP4_KL])
{
    PKCS5_PBKDF2_HMAC(password, strlen(password),
                      salt, OP4_SALT_LEN,
                      PBKDF2_ROUNDS,
                      EVP_sha256(),
                      OP4_KL, out_key);
}

void file_encrypt(fs::path input_file, fs::path output_file, const char *password)
{
    wuk::Random random;
    wuk::byte salt[OP4_SALT_LEN];
    wuk::byte nonce[OP4_NONCE_LEN];
    wuk::byte key[OP4_KL];

    random.bytes(salt, sizeof salt);
    random.bytes(nonce, sizeof nonce);
    derive_key_pbkdf2(password, salt, key);

    std::ifstream fin(input_file, std::ios::binary);
    std::ofstream fout(output_file, std::ios::binary);
    if (!fin || !fout) throw std::runtime_error("file open failed");

    // 写入 salt 和 nonce 到输出文件头部
    fout.write((char*)salt, OP4_SALT_LEN);
    fout.write((char*)nonce, OP4_NONCE_LEN);

    OP4 op4(key);

    printf("Round key:\n");
    print_hex(op4.get_roundkey(), OP4_RKL, 16, true, true);

    wuk::byte plaintext[block_size]{};
    wuk::byte ciphertext[block_size]{};
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

    wuk::byte salt[OP4_SALT_LEN];
    wuk::byte nonce[OP4_NONCE_LEN];
    wuk::byte key[OP4_KL];

    // 从加密文件头部读取 salt 和 nonce
    fin.read((char*)salt, OP4_SALT_LEN);
    fin.read((char*)nonce, OP4_NONCE_LEN);
    derive_key_pbkdf2(password, salt, key);

    OP4 op4(key);
    wuk::byte ciphertext[block_size]{};
    wuk::byte plaintext[block_size]{};
    while (fin.read((char*)ciphertext, block_size) || fin.gcount()) {
        size_t n = fin.gcount();
        op4.ctr_stream(plaintext, ciphertext, n, nonce);
        fout.write((char*)plaintext, n);
    }
}
#endif

#ifdef THREADS_METHOD
#if THREADS_METHOD == 1
void op4_thread(wuk::byte *ciphertext, const wuk::byte *plaintext, wuk::ulong length,
                const wuk::byte key[OP4_KL], const wuk::byte nonce[OP4_NL],
                wuk::u32 thread_count = 4)
{
    std::vector<std::thread> threads;
    wuk::ulong chunk_size = (length / thread_count + OP4_BL - 1) / OP4_BL * OP4_BL; // 对齐到块大小
    wuk::ulong remaining = length;

    for (wuk::u32 i = 0; i < thread_count && remaining > 0; ++i) {
        wuk::ulong current_chunk = std::min(chunk_size, remaining);
        wuk::ulong aligned_chunk = current_chunk - (current_chunk % OP4_BL); // 完整块部分
        wuk::ulong extra_bytes = current_chunk % OP4_BL;                     // 尾部不完整块

        wuk::u32 counter = (i * chunk_size) / OP4_BL; // 精确计算counter

        threads.emplace_back([=]() {
            OP4 op4{key, counter};
            // 处理完整块
            if (aligned_chunk > 0) {
                op4.ctr_stream(ciphertext + i * chunk_size, 
                               plaintext + i * chunk_size, 
                               aligned_chunk, nonce);
            }
            // 处理尾部不完整块（仅最后一个线程可能执行）
            if (extra_bytes > 0 && i == thread_count - 1) {
                wuk::byte last_block[OP4_BL]{0};
                memcpy(last_block, plaintext + i * chunk_size + aligned_chunk, extra_bytes);
                op4.ctr_stream(last_block, last_block, OP4_BL, nonce); // 加密完整块
                memcpy(ciphertext + i * chunk_size + aligned_chunk, last_block, extra_bytes); // 只拷贝所需部分
            }
        });

        remaining -= current_chunk;
    }
    for (auto &t : threads) {
        t.join();
    }
}

#elif THREADS_METHOD == 2
void op4_thread(wuk::byte *ciphertext, const wuk::byte *plaintext, wuk::ulong length,
                         const wuk::byte key[OP4_KL], const wuk::byte nonce[OP4_NL],
                         wuk::u32 thread_count) {
    wuk::ulong chunk_size = ((length / thread_count + OP4_BL - 1) / OP4_BL) * OP4_BL;
    std::vector<std::future<void>> futures;

    for (wuk::u32 i = 0; i < thread_count; ++i) {
        wuk::ulong offset = i * chunk_size;
        wuk::ulong size = (i == thread_count - 1) ? (length - offset) : chunk_size;
        wuk::u32 counter = offset / OP4_BL;

        futures.emplace_back(std::async(std::launch::async, [=]() {
            OP4 op4{key, counter};
            op4.ctr_stream(ciphertext + offset, plaintext + offset, size - (size % OP4_BL), nonce);
            // 处理尾部不完整块（仅最后一个线程）
            if (i == thread_count - 1 && size % OP4_BL != 0) {
                wuk::byte last_block[OP4_BL]{0};
                memcpy(last_block, plaintext + offset + size - (size % OP4_BL), size % OP4_BL);
                op4.ctr_stream(last_block, last_block, OP4_BL, nonce);
                memcpy(ciphertext + offset + size - (size % OP4_BL), last_block, size % OP4_BL);
            }
        }));
    }
    for (auto &f : futures) f.wait();
}

#elif THREADS_METHOD == 3
struct ThreadArgs {
    wuk::byte* ciphertext;
    const wuk::byte* plaintext;
    wuk::ulong offset;
    wuk::ulong size;
    const wuk::byte* key;
    const wuk::byte* nonce;
    wuk::u32 counter;
};

void op4_thread_worker(const ThreadArgs* args) {
    OP4 op4(args->key, args->counter);
    
    // 处理完整块
    wuk::ulong aligned_size = args->size - (args->size % OP4_BL);
    if (aligned_size > 0) {
        op4.ctr_stream(
            args->ciphertext + args->offset,
            args->plaintext + args->offset,
            aligned_size,
            args->nonce
        );
    }

    // 处理尾部不完整块（仅最后一个线程需要）
    if (args->size % OP4_BL != 0) {
        wuk::byte last_block[OP4_BL] = {0};
        memcpy(last_block, 
              args->plaintext + args->offset + aligned_size,
              args->size % OP4_BL);
        
        op4.ctr_stream(last_block, last_block, OP4_BL, args->nonce);
        
        memcpy(args->ciphertext + args->offset + aligned_size,
              last_block,
              args->size % OP4_BL);
    }
}

void op4_thread(wuk::byte* ciphertext, const wuk::byte* plaintext, wuk::ulong length,
            const wuk::byte key[OP4_KL], const wuk::byte nonce[OP4_NL],
            wuk::u32 thread_count = 4) 
{
    // 1. 计算分块参数
    wuk::ulong chunk_size = ((length / thread_count + OP4_BL - 1) / OP4_BL) * OP4_BL;
    std::vector<std::thread> threads;
    std::vector<ThreadArgs> args_list(thread_count);

    // 2. 准备线程参数
    wuk::ulong remaining = length;
    for (wuk::u32 i = 0; i < thread_count && remaining > 0; ++i) {
        ThreadArgs args;
        args.ciphertext = ciphertext;
        args.plaintext = plaintext;
        args.offset = i * chunk_size;
        args.size = std::min(chunk_size, remaining);
        args.key = key;
        args.nonce = nonce;
        args.counter = args.offset / OP4_BL;
        
        args_list[i] = args;
        remaining -= args.size;
    }

    // 3. 启动线程
    for (wuk::u32 i = 0; i < thread_count; ++i) {
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

void op4_single_thread(wuk::byte* ciphertext, const wuk::byte* plaintext, wuk::ulong length,
            const wuk::byte key[OP4_KL], const wuk::byte nonce[OP4_NL])
{
    // 如果不写在函数内部进行初始化的话，计数器会一直更新，导致多线程与单线程加密结果不一致、
    OP4 op4(key, 0);
    op4.ctr_stream(ciphertext, plaintext, length, nonce);
}

void op4_threads()
{
    // 128KB（131072 Bytes）是多线程弱于单线程性能的分水岭
    constexpr wuk::ulong length = static_cast<wuk::ulong>(512ULL * 1024*1024);
    wuk::byte *plaintext = new (std::align_val_t(16), std::nothrow) wuk::byte[length];
    if (!plaintext) {
        throw wuk::Exception(wuk::Error::MEMORY, "op4_threads",
            "failed to allocate for plaintext.");
    }
    wuk::byte *ciphertext = new (std::align_val_t(16), std::nothrow) wuk::byte[length];
    if (!ciphertext) {
        operator delete[](plaintext, std::align_val_t(16));
        throw wuk::Exception(wuk::Error::MEMORY, "op4_threads",
            "failed to allocate for ciphertext.");
    }
    wuk::memory_secure(plaintext, length);
    wuk::memory_secure(ciphertext, length);
    const wuk::byte key[OP4_KL]{0};
    const wuk::byte nonce[OP4_KL]{0};
    wuk::Time timer;

    std::cout << "The length of the encrypted data is: "
              << std::fixed << std::setprecision(2)
              << (static_cast<wuk::f64>(length) / (1024*1024))
              << " MB." << std::endl;
#   if defined(THREADS_METHOD) && ((THREADS_METHOD >= 1) && (THREADS_METHOD <= 3))
    std::cout << "Multi threaded encryption is in progress.." << std::endl;
    wuk::u32 thread_count = std::min(static_cast<wuk::u32>(std::thread::hardware_concurrency()),
                                static_cast<wuk::u32>((length + OP4_BL - 1) / OP4_BL));
    std::cout << "threads count: " << thread_count << std::endl;
    SPEED_TEST(op4_thread(ciphertext, plaintext, length, key, nonce, thread_count));
#   else
    std::cout << "single threaded encryption is in progress.." << std::endl;
    SPEED_TEST(op4_single_thread(ciphertext, plaintext, length, key, nonce));
#   endif
    std::cout << "Plaintext  hexdigest: " << hash_sha256(plaintext,  length) << std::endl;
    std::cout << "Ciphertext hexdigest: " << hash_sha256(ciphertext, length) << std::endl;

    wuk::byte *decrypted = new (std::align_val_t(16), std::nothrow) wuk::byte[length];
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
#endif

#ifdef AVALANCHE_EFFECT_TEST
wuk::u32 bit_diff(const wuk::byte *a, const wuk::byte *b, size_t length)
{
    wuk::u32 diff = 0;

    for (size_t i = 0; i < length; ++i) {
        diff += [](wuk::byte x) -> wuk::u32 {
            wuk::u32 count = 0;
            while (x) {
                count += x & 1;
                x >>= 1;
            }
            return count;
        } (a[i] ^ b[i]);
    }

    return diff;
}

void print_test_info(const wuk::byte *ciphertext1, const wuk::byte *ciphertext2, wuk::ulong length)
{
    std::cout << "Ciphertext1:\t\t\t\t\t\t\tCiphertext2:" << std::endl;
    print_diff_hex((wuk::byte *)ciphertext1, ciphertext2, length, length, 16, false); std::cout << std::endl;

#   ifdef VIEW_HEXDIGEST
    std::cout << "Ciphertext1 hexdigest: " << hash_sha256(ciphertext1, length) << std::endl;
    std::cout << "ciphertext2 hexdigest: " << hash_sha256(ciphertext2, length) << std::endl;
#   endif

    wuk::u32 diff_bits = bit_diff(ciphertext1, (wuk::byte *)ciphertext2, length);
    wuk::f64 diff_ratio = static_cast<wuk::f64>(diff_bits) / (length * 8);
    std::cout << "Diff ratio: " << diff_bits << " / " << (length * 8)
              << " = " << (diff_ratio * 100) << "%" << std::endl;
}

void avalanche_effect_test()
{
    wuk::ulong length    = OP4_BL;
    wuk::byte plaintext1 [OP4_BL]{0};
    wuk::byte plaintext2 [OP4_BL]{0};
    wuk::byte ciphertext1[OP4_BL]{0};
    wuk::byte ciphertext2[OP4_BL]{0};
    wuk::Random random;

    wuk::byte key1  [OP4_KL] {0};
    wuk::byte key2  [OP4_KL] {0};
    wuk::byte nonce1[OP4_NL] {0};
    wuk::byte nonce2[OP4_NL] {0};

    random.bytes(key1,   sizeof key1);
    random.bytes(nonce1, sizeof nonce1);

    constexpr wuk::byte bit = 1 << 0;
    for (wuk::u32 i = 0; i < OP4_KL; ++i) {
        std::cout << "Key test:\n";
        memcpy(key2,   key1,   OP4_KL);
        memcpy(nonce2, nonce1, OP4_NL);
        key2[i] ^= bit;

        OP4 cipher1(key1);
        cipher1.ctr_stream(ciphertext1, plaintext1, length, nonce1);
        OP4 cipher2(key2);
        cipher2.ctr_stream(ciphertext2, plaintext2, length, nonce2);
        print_test_info(ciphertext1, ciphertext2, length);
        std::cout << std::endl;
    }

    for (wuk::u32 i = 0; i < OP4_NL; ++i) {
        std::cout << "Nonce test:\n";
        memcpy(key2,   key1,   OP4_KL);
        memcpy(nonce2, nonce1, OP4_NL);
        nonce2[i] ^= bit;

        OP4 cipher1(key1);
        cipher1.ctr_stream(ciphertext1, plaintext1, length, nonce1);
        OP4 cipher2(key2);
        cipher2.ctr_stream(ciphertext2, plaintext2, length, nonce2);
        print_test_info(ciphertext1, ciphertext2, length);
        std::cout << std::endl;
    }
}
#endif

// python make.py test/op4_test.cc -DWUK_EXPORTS -lsodium -lssl -lcrypto -lbcrypt --std=c++17 -march=native -DTHREADS_METHOD=1

int main()
{
#   ifdef TEST
    xcryption_verification();
#   endif

#   ifdef THREADS_METHOD
    op4_threads();
#   endif

#   ifdef AVALANCHE_EFFECT_TEST
    avalanche_effect_test();
#   endif

    return 0;
}
