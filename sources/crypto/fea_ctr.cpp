#include <crypto/fea.hpp>

static void nonce_add(wByte *counter)
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

void wmkc::crypto::FEA::ctr_xcrypt(wByte *d, wSize n)
{
    wSize i, ks_i;
    wByte ks[WMKC_FEA_BL]{};
    wByte counter[WMKC_FEA_BL]{};

    memcpy(counter, this->nonce.nonce, this->nonce.size);

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
