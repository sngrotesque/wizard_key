#include <crypto/WukOP4.hh>
#include <crypto/WukChaCha20.hh>
// #include <WukBinascii.hh>
#include <WukBuffer.hh>
#include <WukRandom.hh>
#include <WukTime.hh>
#include <WukMisc.hh>

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <new>

#include <openssl/evp.h>

using namespace wuk::crypto;
using namespace wuk::misc;

#define SPEED_TEST(func) \
    func; \
    double start = timer.time(); \
    func; \
    double stop = timer.time(); \
    double taken_time = stop - start; \
    double throughput = length / taken_time / (1024 * 1024); \
    printf("Token time: %.4lf\n", taken_time); \
    printf("Speed: %.2lf MB/s.\n", throughput);

std::string sha256(const wByte *buffer, wSize length)
{
    EVP_MD_CTX   *md_ctx = EVP_MD_CTX_new();
    const EVP_MD *md     = EVP_sha256();
    wByte digest[32] {0};

    EVP_DigestInit_ex(md_ctx, md, nullptr);
    EVP_DigestUpdate(md_ctx, buffer, length);
    EVP_DigestFinal_ex(md_ctx, digest, nullptr);
    EVP_MD_CTX_free(md_ctx);

    return wuk::Buffer(digest, sizeof(digest)).hex();
}

wuk::Buffer get_key(std::string password, wuk::Buffer salt, wU32 length = 32)
{
    wuk::Buffer result;

    PKCS5_PBKDF2_HMAC(password.c_str(), password.length(), salt.get_data(), salt.get_length(),
        501001, EVP_sha256(), length, result.append_write(length));

    return result;
}

wU32 bit_diff(const wByte *a, const wByte *b, size_t length)
{
    wU32 diff = 0;

    for (size_t i = 0; i < length; ++i) {
        diff += [](wByte x) -> wU32 {
            wU32 count = 0;
            while (x) {
                count += x & 1;
                x >>= 1;
            }
            return count;
        } (a[i] ^ b[i]);
    }

    return diff;
}

void print_test_info(const wByte *ciphertext1, const wByte *ciphertext2, wSize length)
{
    std::cout << "Ciphertext1:\t\t\t\t\t\t\tCiphertext2:" << std::endl;
    print_diff_hex((wByte *)ciphertext1, ciphertext2, length, length, 16, false); std::cout << std::endl;

#   ifdef VIEW_HEXDIGEST
    std::cout << "Ciphertext1 hexdigest: " << sha256(ciphertext1, length) << std::endl;
    std::cout << "ciphertext2 hexdigest: " << sha256(ciphertext2, length) << std::endl;
#   endif

    wU32 diff_bits = bit_diff(ciphertext1, (wByte *)ciphertext2, length);
    double diff_ratio = static_cast<double>(diff_bits) / (length * 8);
    std::cout << "Diff ratio: " << diff_bits << " / " << (length * 8)
              << " = " << (diff_ratio * 100) << "%" << std::endl;
}

void avalanche_effect_test()
{
    wSize length    = WukOP4_BL;
    wByte plaintext1 [WukOP4_BL]{0};
    wByte plaintext2 [WukOP4_BL]{0};
    wByte ciphertext1[WukOP4_BL]{0};
    wByte ciphertext2[WukOP4_BL]{0};
    wuk::Random random;

    wByte key1  [WukOP4_KL] {0};
    wByte key2  [WukOP4_KL] {0};
    wByte nonce1[WukOP4_NL] {0};
    wByte nonce2[WukOP4_NL] {0};

    random.urandom(key1,   sizeof key1);
    random.urandom(nonce1, sizeof nonce1);

    constexpr wByte bit = 1 << 7;
    for (wU32 i = 0; i < WukOP4_KL; ++i) {
        std::cout << "Key test:\n";
        memcpy(key2,   key1,   WukOP4_KL);
        memcpy(nonce2, nonce1, WukOP4_NL);
        key2[i] ^= bit;

        WukOP4 cipher1(key1);
        cipher1.ctr_stream(ciphertext1, plaintext1, length, nonce1);
        WukOP4 cipher2(key2);
        cipher2.ctr_stream(ciphertext2, plaintext2, length, nonce2);
        print_test_info(ciphertext1, ciphertext2, length);
        std::cout << std::endl;
    }

    for (wU32 i = 0; i < WukOP4_NL; ++i) {
        std::cout << "Nonce test:\n";
        memcpy(key2,   key1,   WukOP4_KL);
        memcpy(nonce2, nonce1, WukOP4_NL);
        nonce2[i] ^= bit;

        WukOP4 cipher1(key1);
        cipher1.ctr_stream(ciphertext1, plaintext1, length, nonce1);
        WukOP4 cipher2(key2);
        cipher2.ctr_stream(ciphertext2, plaintext2, length, nonce2);
        print_test_info(ciphertext1, ciphertext2, length);
        std::cout << std::endl;
    }
}

void chacha20_test()
{
    wuk::Random random;
    wuk::Time timer;
    wByte key  [WukCC20_KL] {0};
    wByte nonce[WukCC20_NL] {0};

    random.urandom(key,   sizeof key);
    random.urandom(nonce, sizeof nonce);

    WukChaCha20 cc20(key);

    wSize  length     = 512ULL * 1024 * 1024;
    wByte *plaintext  = new (std::align_val_t(16), std::nothrow) wByte[length];
    wByte *ciphertext = new (std::align_val_t(16), std::nothrow) wByte[length];

    SPEED_TEST(cc20.crypto_stream(ciphertext, plaintext, length, nonce));

    operator delete[] (ciphertext, std::align_val_t(16));
    operator delete[] (plaintext, std::align_val_t(16));
}

// python make.py test/crypto_test.cc -DWUK_EXPORTS -lsodium -lssl -lcrypto -lbcrypt --std=c++17 -march=native -DLIBSODIUM_SUPPORT

int main()
{
    avalanche_effect_test();

    return 0;
}
