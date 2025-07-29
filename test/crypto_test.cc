#include <crypto/WukOP4.hh>
#include <crypto/WukChaCha20.hh>
#include <WukBinascii.hh>
#include <WukBuffer.hh>
#include <WukTime.hh>
#include <WukMisc.hh>

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <new>

#include <openssl/evp.h>
#include <sodium.h>
#include <zlib.h>

using namespace wuk::crypto;
using namespace wuk::misc;

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

void print_test_info(const wByte *ciphertext, const wByte *plaintext, wSize length)
{
    std::cout << "Plaintext:\t\t\t\t\t\t\tCiphertext:" << std::endl;
    print_diff_hex((wByte *)plaintext, ciphertext, length, length, 16, true); std::cout << std::endl;

#   ifdef VIEW_HEXDIGEST
    std::cout << "Plaintext  hexdigest: " << sha256(plaintext, length) << std::endl;
    std::cout << "ciphertext hexdigest: " << sha256(ciphertext, length) << std::endl;
#   endif

    wU32 diff_bits = bit_diff(ciphertext, (wByte *)plaintext, length);
    double diff_ratio = static_cast<double>(diff_bits) / (length * 8);
    std::cout << "Diff ratio: " << diff_bits << " / " << (length * 8)
              << " = " << (diff_ratio * 100) << "%" << std::endl;
}

void test()
{
    const char *original_message = "what the fuck?!";
    const wSize length = strlen(original_message);

    wByte *plaintext  = (wByte *)original_message;
    wByte *ciphertext = wuk::m_alloc<wByte *>(length);
    if (!ciphertext) {
        std::cerr << "ciphertext allocate error." << std::endl;
        return;
    }

    for (wU32 r = 0; r < 10; ++r) {
        wByte key  [WukOP4_KL] {0};
        wByte nonce[WukOP4_NL] {0};
        randombytes_buf(key,   sizeof key);
        randombytes_buf(nonce, sizeof nonce);

        std::cout << "Cipher algorithm: OP4." << std::endl;
        WukOP4 op4(key);
        op4.ctr_stream(ciphertext, plaintext, length, nonce);
        print_test_info(ciphertext, plaintext, length);
        std::cout << "\n";

        std::cout << "Cipher algorithm: ChaCha20." << std::endl;
        WukChaCha20 cc20(key);
        cc20.crypto_stream(ciphertext, plaintext, length, nonce);
        print_test_info(ciphertext, plaintext, length);
        std::cout << "\n\n";
    }

    wuk::m_free(ciphertext);
}

int main()
{
    test();

    return 0;
}
