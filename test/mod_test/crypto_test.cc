#include <crypto/WukOP4.hh>
#include <crypto/WukChaCha20.hh>
// #include <WukBinascii.hh>
#include <WukBuffer.hh>
#include <WukRandom.hh>
#include <WukTime.hh>
#include <WukMisc.hh>

#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <new>

#include <openssl/evp.h>

using namespace wuk::crypto;
using namespace wuk::misc;
namespace fs = std::filesystem;

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

void chacha20_test()
{
    wuk::WukRandom random;
    wuk::WukTime timer;
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
    chacha20_test();

    return 0;
}
