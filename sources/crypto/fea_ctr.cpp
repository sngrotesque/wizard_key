#include <crypto/fea.hpp>

static void nonce_add(wByte *counter)
{
    for(wS32 ctr_i = (WUK_FEA_BL - 1); ctr_i >= 0; --ctr_i) {
        if(*(counter + ctr_i) == 0xff) {
            *(counter + ctr_i) = 0x00;
        } else {
            ++(*(counter + ctr_i));
            break;
        }
    }
}

void wuk::crypto::FEA::ctr_xcrypt(wByte *d, wSize n)
{
    wSize i, ks_i;
    wByte ks[WUK_FEA_BL]{};

    // // 使用这种方法的话，可能让nonce变得不安全，因为counter初始化于用户一开始的，并且
    // // 即使调用过一次ctr_xcrypt也没用，因为会根据一开始的counter再次初始化当前计数器。
    // wByte counter[WUK_FEA_BL]{};
    // memcpy(counter, this->counter.counter, sizeof(this->counter.counter));

    // 使用这种方法的话，没法在加密后立即使用相同的FEA上下文进行解密
    wByte *counter = this->counter.counter;

    for(i = 0, ks_i = WUK_FEA_BL; i < n; ++i, ++ks_i) {
        if(ks_i == WUK_FEA_BL) {
            memcpy(ks, counter, WUK_FEA_BL);
            this->cipher(ks, this->roundKey);

            nonce_add(counter);

            ks_i = 0;
        }
        *(d + i) ^= *(ks + ks_i);
    }
}
