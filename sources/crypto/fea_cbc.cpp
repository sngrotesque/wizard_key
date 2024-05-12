#include <crypto/fea.hpp>

wVoid wmkc::crypto::fea::cbc_encrypt(wByte *p, wSize n)
{
    wByte roundIv[WMKC_FEA_BL];

    memcpy(roundIv, this->iv, WMKC_FEA_BL);
    for(wU32 i = 0; i < n; i += WMKC_FEA_BL) {
        this->xorWithIV(p + i, roundIv);
        this->cipher(p + i, this->roundKey);
        memcpy(roundIv, p + i, WMKC_FEA_BL);
    }
}

wVoid wmkc::crypto::fea::cbc_decrypt(wByte *c, wSize n)
{
    wByte roundIv[WMKC_FEA_BL];
    wByte roundBuffer[WMKC_FEA_BL];

    memcpy(roundIv, this->iv, WMKC_FEA_BL);
    for(wU32 i = 0; i < n; i += WMKC_FEA_BL) {
        memcpy(roundBuffer, c + i, WMKC_FEA_BL);
        this->invCipher(c + i, this->roundKey);
        this->xorWithIV(c + i, roundIv);
        memcpy(roundIv, roundBuffer, WMKC_FEA_BL);
    }
}
