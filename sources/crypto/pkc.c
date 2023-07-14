#include <crypto/pkc.h>

void wmkcPKC_encrypt(wmkcByte *buf, wmkcByte *pk)
{
    wmkcFast wmkc_u32 i;
    wmkcSize pk_n;

    wmkcStruct_unpack(">Q", &pk_n, pk);

    for(i = 0; i < 8; ++i) {
        buf[i] ^= pk[i];
    }

}


