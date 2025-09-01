#include <crypto/WukOP4.hh>
#include <crypto/WukChaCha20.hh>
#include <crypto/WukHash.hh>
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

wuk::Buffer derive_key(const std::string &password, const wuk::Buffer &salt, wuk::u32 length = 32)
{
    wuk::Buffer result;

    PKCS5_PBKDF2_HMAC(password.c_str(), password.length(), salt.get_data(), salt.get_length(),
        201001, EVP_sha256(), length, result.append(length));

    return result;
}

void op4_encryption_test()
{
    auto keyWithNonce = derive_key("12345678", {"abcdef0123456789"}, OP4_KL + OP4_NL);
    const wuk::byte *key = keyWithNonce.get_data();
    const wuk::byte *nonce = keyWithNonce.get_data() + OP4_KL;

    OP4 op4(key);

    const char *original = {
        "hello, world.\n"
        "This is testing.\n"
    };
    wuk::ulong length = strlen(original);
    const wuk::byte *plaintext = reinterpret_cast<const wuk::byte *>(original);
    wuk::byte *ciphertext = wuk::m_calloc<wuk::byte>(length);

    op4.ctr_stream(ciphertext, plaintext, length, nonce);

    std::cout << "Plaintext:\t\t\t\t\t\t\tCiphertext:\n";
    print_diff_hex(plaintext, ciphertext, length, length, 16, true);

    std::cout << "Plaintext: " << get_pybytes(plaintext, length, false) << std::endl;
    std::cout << "Ciphertext: " << get_pybytes(ciphertext, length, false) << std::endl;

    wuk::m_free(ciphertext);
}

// python make.py test/crypto_test.cc -lssl -lcrypto

int main()
{
    op4_encryption_test();

    return 0;
}
