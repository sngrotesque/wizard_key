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

// #define WEAK_KEY_TEST
// #define XCRYPTION_TEST
// #define THREADS_METHOD 3
// #define AVALANCHE_EFFECT 1
// #define CUSTOM_TEST

namespace fs = std::filesystem;

using namespace wuk::crypto;
using namespace wuk::misc;

constexpr wuk::u32 OP4_SALT_LEN  = OP4_BL;
constexpr wuk::u32 OP4_NONCE_LEN = OP4_NL;
constexpr wuk::u32 PBKDF2_ROUNDS = 415411;

constexpr wuk::u32 decryption_error = 777777777;

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

static wuk::Random wrand;

std::string hash_sha256(const wuk::byte *buffer, wuk::ulong length)
{
    Hashlib<HashlibType::SHA_256> hash;
    hash.update(buffer, length);
    return hash.hexdigest();
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
    wuk::byte salt[OP4_SALT_LEN];
    wuk::byte nonce[OP4_NONCE_LEN];
    wuk::byte key[OP4_KL];

    wrand.bytes(salt, sizeof salt);
    wrand.bytes(nonce, sizeof nonce);
    derive_key_pbkdf2(password, salt, key);

    std::ifstream fin(input_file, std::ios::binary);
    std::ofstream fout(output_file, std::ios::binary);
    if (!fin || !fout) throw std::runtime_error("file open failed");

    // 写入 salt 和 nonce 到输出文件头部
    fout.write((char*)salt, OP4_SALT_LEN);
    fout.write((char*)nonce, OP4_NONCE_LEN);

    OP4 op4(key);

    printf("Round key:\n");
    print_hex(op4.get_round_key(), OP4_RKL, 16, true, true);

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

#ifdef WEAK_KEY_TEST
template <bool timer>
void weak_key_test(wuk::u32 count = 10000000)
{
    wuk::byte master_key_left[OP4_KL] {0};
    wuk::byte master_key_right[OP4_KL] {0};
    wuk::f64 total_diff_bits{0};

    wuk::Time time;
    double start{0}, stop{0};

    if constexpr (timer) {
        start = time.time<double>();
    }

    for (wuk::u32 i = 0; i < count; ++i) {
        // 初始化左边的主密钥
        wrand.bytes(master_key_left, OP4_KL);
        // 初始化右边的主密钥（只根据左边的主密钥随机改变1bit）
        memcpy(master_key_right, master_key_left, OP4_KL);

        wuk::u32 index = wrand.randint(0, OP4_KL - 1);
        wuk::u32 bit = wrand.randint(0, 7);
        master_key_right[index] ^= (1 << bit);

        // std::cout << fmt::format("Made a modification to the {1} bit of the {0} byte.", index+1, 8-bit) << std::endl;
        // std::cout << "Master key (left):\t\t\t\t\t\tMaster key (Right):\n";
        // print_diff_hex(master_key_left, master_key_right, OP4_KL, OP4_KL, OP4_BL, true);

        OP4 cipher_left(master_key_left);
        OP4 cipher_right(master_key_right);

        const wuk::byte *round_key_left  = cipher_left.get_round_key();
        const wuk::byte *round_key_right = cipher_right.get_round_key();

        // std::cout << "Round key (left):\t\t\t\t\t\tRound key (Right):\n";
        // print_diff_hex(round_key_left, round_key_right, OP4_RKL, OP4_RKL, OP4_BL, true);

        wuk::u32 bit_diff_count = bit_diff(round_key_left, round_key_right, OP4_RKL);
        wuk::f64 bit_diff_ratio = (static_cast<wuk::f64>(bit_diff_count) / (OP4_RKL * 8));

        total_diff_bits += bit_diff_ratio;
    }

    if constexpr (timer) {
        stop = time.time<double>();
        std::cout << fmt::format("Time taken: {0:.4f}", (stop-start)) << std::endl;
    }

    std::cout << fmt::format(
        "The number of samples used for the round key avalanche effect test: {0}.", count) << std::endl;
    std::cout << fmt::format(
        "Avalanche effect test of round key, bit difference rate: {0:.4f} %.",
        ((total_diff_bits / count) * 100)) << std::endl;
}
#endif

#ifdef XCRYPTION_TEST
void xcryption_verification()
{
    wuk::byte key[OP4_KL]   {0};
    wuk::byte iv[OP4_BL]    {0};
    wuk::byte nonce[OP4_NL] {0};

    constexpr size_t length = OP4_BL * 3;
    wuk::byte plaintext[length]{
        0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
        0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
        0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
        0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
        0x00, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
        0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
    };
    wuk::byte ciphertext[length]{0};
    wuk::byte decrypted[length]{0};

    wrand.bytes(key, OP4_KL);
    wrand.bytes(iv, OP4_BL);
    wrand.bytes(nonce, OP4_NL);

    OP4 op4(key);

    std::cout << "Master key:\n";
    print_hex(key, OP4_KL, OP4_BL, true, true);

    std::cout << "Master Nonce:\n";
    print_hex(nonce, OP4_NL, OP4_BL, true, true);

    std::cout << "Master IV:\n";
    print_hex(iv, OP4_BL, OP4_BL, true, true);

    std::cout << "Round key\n";
    print_hex(op4.get_round_key(), OP4_RKL, OP4_BL, true, true);

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

/**
 * <-------------- 明文扰动测试 -------------->
 * === OP4 Avalanche Effect Test Results ===
 * Samples: 10000000
 * Average changed bits: 64.0017 (Ideal: 64)
 * Average ratio: 50.0013%
 * Standard deviation: 5.65618
 * 
 * SAC Statistics:
 *   Min ratio: 49.9508%
 *   Max ratio: 50.0381%
 *   Avg ratio: 50.0013%
 * 
 * SAC Test (First 10 bits):
 * Bit 0: 49.9756%
 * Bit 1: 50.0041%
 * Bit 2: 50.0144%
 * Bit 3: 49.9734%
 * Bit 4: 50.016%
 * Bit 5: 50.0006%
 * Bit 6: 49.9694%
 * Bit 7: 50.001%
 * Bit 8: 50.0267%
 * Bit 9: 49.9733%
 * 
 * 95% Confidence Interval: [63.9982, 64.0052]
 * 
 * <-------------- 密钥扰动测试 -------------->
 * === OP4 Avalanche Effect Test Results ===
 * Samples: 10000000
 * Average changed bits: 64.0016 (Ideal: 64)
 * Average ratio: 50.0013%
 * Standard deviation: 5.6573
 * 
 * SAC Statistics:
 *   Min ratio: 49.9567%
 *   Max ratio: 50.039%
 *   Avg ratio: 50.0013%
 * 
 * SAC Test (First 10 bits):
 * Bit 0: 49.9771%
 * Bit 1: 50.0103%
 * Bit 2: 50.017%
 * Bit 3: 50.0121%
 * Bit 4: 49.9834%
 * Bit 5: 50.0003%
 * Bit 6: 49.9885%
 * Bit 7: 49.9812%
 * Bit 8: 49.9952%
 * Bit 9: 50.0169%
 * 
 * 95% Confidence Interval: [63.9981, 64.0051]
 */
#ifdef AVALANCHE_EFFECT
void avalanche_effect_test(wuk::u32 sample_count = 10000000)
{
    wuk::byte ciphertext1[OP4_BL]{};
    wuk::byte ciphertext2[OP4_BL]{};

    wuk::f64 total_bit_diff = 0.0;
    std::vector<wuk::u32> bit_change_count(OP4_BL * 8, 0);
    std::vector<wuk::u32> bit_diff_history;
    bit_diff_history.reserve(sample_count);

    for (wuk::u32 i = 0; i < sample_count; ++i) {
#   if (AVALANCHE_EFFECT == 1)
        wuk::byte plaintext1[OP4_BL]{};
        wuk::byte plaintext2[OP4_BL]{};
        wuk::byte key[OP4_KL]{};

        wrand.bytes(plaintext1, OP4_BL);
        wrand.bytes(key, OP4_KL);

        memcpy(plaintext2, plaintext1, OP4_BL);
        wuk::u32 byte_index = wrand.randint(0, OP4_BL - 1);
        wuk::u32 bit_index = wrand.randint(0, 7);
        plaintext2[byte_index] ^= (1 << bit_index);

        OP4 cipher(key);
        cipher.ecb_encrypt(ciphertext1, plaintext1, OP4_BL);
        cipher.ecb_encrypt(ciphertext2, plaintext2, OP4_BL);
#   elif (AVALANCHE_EFFECT == 2)
        wuk::byte plaintext[OP4_BL]{};
        wuk::byte key1[OP4_KL]{};
        wuk::byte key2[OP4_KL]{};

        wrand.bytes(plaintext, OP4_BL);
        wrand.bytes(key1, OP4_KL);

        memcpy(key2, key1, OP4_KL);
        wuk::u32 byte_index = wrand.randint(0, OP4_KL - 1);
        wuk::u32 bit_index = wrand.randint(0, 7);
        key2[byte_index] ^= (1 << bit_index);

        OP4 cipher1(key1);
        cipher1.ecb_encrypt(ciphertext1, plaintext, OP4_BL);
        OP4 cipher2(key2);
        cipher2.ecb_encrypt(ciphertext2, plaintext, OP4_BL);
#       endif

        wuk::u32 diff_bits = bit_diff(ciphertext1, ciphertext2, OP4_BL);
        total_bit_diff += diff_bits;
        bit_diff_history.push_back(diff_bits);

        for (size_t byte_idx = 0; byte_idx < OP4_BL; ++byte_idx) {
            wuk::byte diff_byte = ciphertext1[byte_idx] ^ ciphertext2[byte_idx];
            for (int bit_idx = 0; bit_idx < 8; ++bit_idx) {
                if (diff_byte & (1 << bit_idx)) {
                    size_t global_bit_idx = byte_idx * 8 + bit_idx;
                    bit_change_count[global_bit_idx]++;
                }
            }
        }
    }

    // 计算统计数据
    constexpr wuk::f64 ideal_diff_bits = OP4_BL * 8 / 2.0;
    wuk::f64 average_diff_bits = total_bit_diff / sample_count;
    wuk::f64 variance = 0.0;
    for (auto diff : bit_diff_history) {
        variance += (diff - average_diff_bits) * (diff - average_diff_bits);
    }
    variance /= sample_count;
    wuk::f64 std_dev = std::sqrt(variance);

    // 计算SAC统计
    wuk::f64 sac_min = 100.0, sac_max = 0.0, sac_avg = 0.0;
    for (auto count : bit_change_count) {
        wuk::f64 ratio = static_cast<wuk::f64>(count) / sample_count * 100.0;
        sac_avg += ratio;
        sac_min = std::min(sac_min, ratio);
        sac_max = std::max(sac_max, ratio);
    }
    sac_avg /= bit_change_count.size();

    // 输出结果
    std::cout << "\n=== OP4 Avalanche Effect Test Results ===\n";
    std::cout << "Samples: " << sample_count << "\n";
    std::cout << "Average changed bits: " << average_diff_bits 
              << " (Ideal: " << ideal_diff_bits << ")\n";
    std::cout << "Average ratio: " << (average_diff_bits / ideal_diff_bits * 50.0) << "%\n";
    std::cout << "Standard deviation: " << std_dev << "\n";

    std::cout << "\nSAC Statistics:\n";
    std::cout << "  Min ratio: " << sac_min << "%\n";
    std::cout << "  Max ratio: " << sac_max << "%\n";
    std::cout << "  Avg ratio: " << sac_avg << "%\n";

    std::cout << "\nSAC Test (First 10 bits):\n";
    for (size_t i = 0; i < 10 && i < bit_change_count.size(); ++i) {
        double ratio = static_cast<double>(bit_change_count[i]) / sample_count * 100.0;
        std::cout << "Bit " << i << ": " << ratio << "%\n";
    }

    // 计算置信区间(95%)
    wuk::f64 margin_error = 1.96 * std_dev / std::sqrt(sample_count);
    std::cout << "\n95% Confidence Interval: [" 
              << (average_diff_bits - margin_error) << ", " 
              << (average_diff_bits + margin_error) << "]\n";
}
#endif

#ifdef CUSTOM_TEST
void custom_test()
{
    wuk::byte key[OP4_KL]{
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
    };
    wuk::byte nonce[OP4_NL]{
        0x80, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b
    };

    char _content[] = {
        "GET / HTTP/1.1\r\n"
        "Host: sngrotesque.com\r\n"
        "Accept: */*\r\n"
        "User-Agent: android\r\n\r\n"
    };
    constexpr wuk::ulong length = sizeof(_content) - 1;
    wuk::byte *plaintext = reinterpret_cast<wuk::byte *>(_content);
    wuk::byte *ciphertext = new (std::nothrow) wuk::byte[length];
    if (!ciphertext) {
        std::cerr << "failed to new ciphertext.\n";
        return;
    }

    OP4 cipher(key);

    cipher.ctr_stream(ciphertext, plaintext, length, nonce);

    std::cout << "Plaintext:\t\t\t\t\t\t\tCiphertext:\n";
    print_diff_hex(plaintext, ciphertext, length, length, 16, true);

    delete[] ciphertext;
}
#endif

/*
 *  python py/exec.py test/mod_test/op4_test.cc -lssl -lcrypto \
 *          [-lbcrypt \
 *          -DWEAK_KEY_TEST \
 *          -DXCRYPTION_TEST \
 *          -DTHREADS_METHOD=1 \
 *          -DAVALANCHE_EFFECT=1 \
 *          -DCUSTOM_TEST
 */
int main(int argc, char **argv)
{
#   ifdef WEAK_KEY_TEST
    std::cout << "================================ Weak key test ================================\n";
    weak_key_test<true>();
#   endif

#   ifdef XCRYPTION_TEST
    std::cout << "================================ xcryption test ================================\n";
    xcryption_verification();
#   endif

#   ifdef THREADS_METHOD
    std::cout << "================================ threads test ================================\n";
    op4_threads();
#   endif

#   ifdef AVALANCHE_EFFECT
    std::cout << "================================ avalanche effect test ================================\n";
    avalanche_effect_test();
#   endif

#   ifdef CUSTOM_TEST
    std::cout << "================================ custom test ================================\n";
    custom_test();
#   endif

    return 0;
}
