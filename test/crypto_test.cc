#include <WukBuffer.hh>
#include <WukTime.hh>
#include <WukMisc.hh>

#include <iostream>
#include <iomanip>
#include <new>

#include <openssl/evp.h>
#include <sodium.h>

const wByte *chacha20_key   = (wByte *)"bbcdef0123456789abcdef0123456789";
const wByte *chacha20_nonce = (wByte *)"abcdef012345";

class CryptoTest {
private:
    wuk::Buffer sha256(wuk::Buffer buffer)
    {
        wByte digest[32]{};

        const EVP_MD *md = EVP_sha256();
        EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();

        EVP_DigestInit_ex(md_ctx, md, nullptr);
        EVP_DigestUpdate(md_ctx, buffer.get_data(), buffer.get_length());
        EVP_DigestFinal_ex(md_ctx, digest, nullptr);
        EVP_MD_CTX_free(md_ctx);

        return wuk::Buffer(digest, sizeof(digest));
    }

    wuk::Buffer get_key(std::string password)
    {
        wuk::Buffer pass_salt = sha256(password);
        wuk::Buffer result;

        PKCS5_PBKDF2_HMAC(password.c_str(), password.length(), pass_salt.get_data(), pass_salt.get_length(), 501001, EVP_sha256(), 128, result.append_write(128));

        return result;
    }
public:
    CryptoTest() = default;
};

void chacha20_speed_test(size_t length)
{
    if (sodium_init() < 0) {
        printf("libsodium 初始化失败！\n");
        return;
    }
    wuk::Time timer;

    // 明文和密文
    wByte *plaintext = new (std::align_val_t(32)) wByte[length];
    wByte *ciphertext = new (std::align_val_t(32)) wByte[length];

    wuk::memory_zero(plaintext, length);
    wuk::memory_zero(ciphertext, length);

    double start = timer.time();
    crypto_stream_chacha20_xor(ciphertext, plaintext, length, chacha20_nonce, chacha20_key);
    double stop = timer.time();
    printf("Time taken: %.4f\nCiphertext[0]: %02x\n", (stop-start), ciphertext[0]);

    operator delete[] (ciphertext, std::align_val_t(32));
    operator delete[] (plaintext, std::align_val_t(32));
}

int main()
{
    chacha20_speed_test(1ULL * 1024 * 1024 * 1024);
    return 0;
}
