#include <crypto/fea.hpp>

static wVoid nonce_add(wByte *counter)
{
    for(wS32 ctr_i = (WMKC_FEA_BL - 1); ctr_i >= 0; --ctr_i) {
        if(*(counter + ctr_i) == 0xff) {
            *(counter + ctr_i) = 0x00;
        } else {
            ++(*(counter + ctr_i));
            break;
        }
    }
}

wVoid wmkc::crypto::fea::ctr_xcrypt(wByte *d, wSize n)
{
    wSize i, ks_i;
    wByte counter[WMKC_FEA_BL];
    wByte ks[WMKC_FEA_BL] = {0};

    wmkc::memory_zero(counter, WMKC_FEA_BL);
    memcpy(counter, this->iv, WMKC_FEA_BL - 1); // 确保得到的Nonce足够长

    for(i = 0, ks_i = WMKC_FEA_BL; i < n; ++i, ++ks_i) {
        if(ks_i == WMKC_FEA_BL) {
            memcpy(ks, counter, WMKC_FEA_BL);
            this->cipher(ks, this->roundKey);

            nonce_add(counter);

            ks_i = 0;
        }
        *(d + i) ^= *(ks + ks_i);
    }
}
