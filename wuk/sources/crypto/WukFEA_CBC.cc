#include <crypto/WukFEA.hh>

void wuk::crypto::FEA::cbc_encrypt(wByte *p, wSize n)
{
    wByte roundIv[WUK_FEA_BL];

    memcpy(roundIv, this->iv, WUK_FEA_BL);
    for(wU32 i = 0; i < n; i += WUK_FEA_BL) {
        this->xor_with_iv(p + i, roundIv);
        this->cipher(p + i, this->roundKey);
        memcpy(roundIv, p + i, WUK_FEA_BL);
    }
}

void wuk::crypto::FEA::cbc_decrypt(wByte *c, wSize n)
{
    wByte roundIv[WUK_FEA_BL];
    wByte roundBuffer[WUK_FEA_BL];

    memcpy(roundIv, this->iv, WUK_FEA_BL);
    for(wU32 i = 0; i < n; i += WUK_FEA_BL) {
        memcpy(roundBuffer, c + i, WUK_FEA_BL);
        this->inv_cipher(c + i, this->roundKey);
        this->xor_with_iv(c + i, roundIv);
        memcpy(roundIv, roundBuffer, WUK_FEA_BL);
    }
}
