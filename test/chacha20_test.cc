#include <WukMisc.hh>
#include <WukTime.hh>

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>

#include <sodium.h>

const wByte *chacha20_key   = (wByte *)"bbcdef0123456789abcdef0123456789";
const wByte *chacha20_nonce = (wByte *)"abcdef012345";

void speed_test(size_t length)
{
    // 初始化 libsodium
    if (sodium_init() < 0) {
        printf("libsodium 初始化失败！\n");
        return;
    }
    wuk::Time timer;

    // 明文和密文
    wByte *plaintext = new wByte[length];
    wByte *ciphertext = new wByte[length];

    wuk::memory_zero(plaintext, length);
    wuk::memory_zero(ciphertext, length);

    double start = timer.time();
    crypto_stream_chacha20_xor(ciphertext, plaintext, length, chacha20_nonce, chacha20_key);
    double stop = timer.time();
    printf("Time taken: %.4f\nCiphertext[0]: %02x\n", (stop-start), ciphertext[0]);

    delete[] ciphertext;
    delete[] plaintext;
}

int main()
{
    speed_test(1024ULL * pow(1024, 2));

    return 0;
}
