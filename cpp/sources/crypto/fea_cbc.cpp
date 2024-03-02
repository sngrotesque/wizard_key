#include <crypto/fea.hpp>

wmkcVoid wmkcCrypto::wmkcFEA::cbc_encrypt(wmkcByte *p, wmkcSize n)
{
    wmkcByte roundIv[WMKC_FEA_BLOCKLEN];

    memcpy(roundIv, this->iv, WMKC_FEA_BLOCKLEN);
    for(wmkc_u32 i = 0; i < n; i += WMKC_FEA_BLOCKLEN) {
        this->xorWithIV(p + i, roundIv);
        this->cipher(p + i, this->roundKey);
        memcpy(roundIv, p + i, WMKC_FEA_BLOCKLEN);
    }
}

wmkcVoid wmkcCrypto::wmkcFEA::cbc_decrypt(wmkcByte *c, wmkcSize n)
{
    wmkcByte roundIv[WMKC_FEA_BLOCKLEN];
    wmkcByte roundBuffer[WMKC_FEA_BLOCKLEN];

    memcpy(roundIv, this->iv, WMKC_FEA_BLOCKLEN);
    for(wmkc_u32 i = 0; i < n; i += WMKC_FEA_BLOCKLEN) {
        memcpy(roundBuffer, c + i, WMKC_FEA_BLOCKLEN);
        this->invCipher(c + i, this->roundKey);
        this->xorWithIV(c + i, roundIv);
        memcpy(roundIv, roundBuffer, WMKC_FEA_BLOCKLEN);
    }
}
