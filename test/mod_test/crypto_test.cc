#include <crypto/WukOP4.hh>
#include <crypto/WukChaCha20.hh>
// #include <WukBinascii.hh>
#include <WukBuffer.hh>
#include <WukMemory.hh>
#include <WukTime.hh>
#include <WukMisc.hh>

#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <new>

#include <openssl/rand.h>
#include <openssl/evp.h>

using namespace wuk::crypto;
using namespace wuk::misc;
namespace fs = std::filesystem;

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
        201001, EVP_sha256(), length, result.append_write(length));

    return result;
}

void op4_encryption_test()
{
    auto keyWithNonce = get_key("12345678", {"abcdef0123456789"}, OP4_KL + OP4_NL);
    const wByte *key = keyWithNonce.get_data();
    const wByte *nonce = keyWithNonce.get_data() + OP4_KL;

    OP4 op4(key);

    const char *original = {
        "hello, world.\n"
        "This is testing.\n"
    };
    wSize length = strlen(original);
    const wByte *plaintext = reinterpret_cast<const wByte *>(original);
    wByte *ciphertext = wuk::m_calloc<wByte>(length);

    op4.ctr_stream(ciphertext, plaintext, length, nonce);

    std::cout << "Plaintext:\t\t\t\t\t\t\tCiphertext:\n";
    print_diff_hex(plaintext, ciphertext, length, length, 16, true);

    std::cout << "Plaintext: " << get_pybytes(plaintext, length, false) << std::endl;
    std::cout << "Ciphertext: " << get_pybytes(ciphertext, length, false) << std::endl;

    wuk::m_free(ciphertext);
}

// python make.py test/crypto_test.cc -DWUK_EXPORTS -lssl -lcrypto -march=native

int main()
{
    op4_encryption_test();

    return 0;
}
