#include <crypto/WukFEA.hh>

void wuk::crypto::FEA::ctr_xcrypt(wByte *d, wSize n)
{
    wSize i, ks_i;
    wByte ks[WUK_FEA_BL]{}; // key stream

    wByte *counter = this->counter.get();

    for(i = 0, ks_i = WUK_FEA_BL; i < n; ++i, ++ks_i) {
        if(ks_i == WUK_FEA_BL) {
            memcpy(ks, counter, WUK_FEA_BL);
            this->cipher(ks, this->roundKey);

            this->counter.step_up();

            ks_i = 0;
        }
        *(d + i) ^= *(ks + ks_i);
    }
}
