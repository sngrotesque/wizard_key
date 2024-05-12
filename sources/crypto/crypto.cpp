#include <crypto/crypto.hpp>

wVoid wmkc::crypto::generateSbox(wByte *sbox)
{
    for (int i = 0; i < 256; i++) {
        sbox[i] = i;
    }
    std::random_device rd;
    std::mt19937 g(rd());
    for (int i = 0; i < 8; i++) {
        std::shuffle(sbox, sbox + 256, g);
    }
}

wVoid wmkc::crypto::generateRSbox(wByte *rsbox, wByte *sbox)
{
    for(wU32 x = 0; x < 256; ++x) {
        rsbox[sbox[x]] = x;
    }
}
