#include <WukTime.hh>
#include <WukTime.cc>

#include <sodium.h>
#include <stdio.h>

int main() {
    // 初始化 libsodium
    if (sodium_init() < 0) {
        printf("libsodium 初始化失败！\n");
        return 1;
    }
    wuk::Time timer;

    // 密钥和随机数
    unsigned char key[crypto_stream_chacha20_KEYBYTES];
    unsigned char nonce[crypto_stream_chacha20_NONCEBYTES];
    randombytes_buf(key, sizeof(key));
    randombytes_buf(nonce, sizeof(nonce));

    // 明文和密文
    constexpr wSize length = 1024 * 1024 * 1024;
    wByte *plaintext = new wByte[length];
    wByte *ciphertext = new wByte[length];

    double start = timer.time();
    crypto_stream_chacha20_xor(ciphertext, plaintext, length, nonce, key);
    double stop = timer.time();
    printf("Time taken: %.4f\nCiphertext[0]: %02x\n", (stop-start), ciphertext[0]);

    delete[] ciphertext;
    delete[] plaintext;
    return 0;
}