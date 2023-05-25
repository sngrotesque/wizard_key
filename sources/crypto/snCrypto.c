#include <crypto/snCrypto.h>

WMKC_PUBLIC(wmkcVoid) snCrypto_sbox_init WMKC_OPEN_API
WMKC_OF((wmkcByte *sbox))
{
    snRand_Seed();
    static wmkc_u32 r, x, left, right;
    static wmkcByte swap;

    for(x = 0; x < 256; ++x)
        sbox[x] = x;

    for(r = 0; r < 16; ++r) {
        for(x = 0; x < 256; ++x) {
            do {
                left  = snRand_Randint(0, 255);
                right = snRand_Randint(0, 255);
            } while(left == right);
            swap = sbox[left];
            sbox[left] = sbox[right];
            sbox[right] = swap;
        }
    }
}

WMKC_PUBLIC(wmkcVoid) snCrypto_rsbox_init WMKC_OPEN_API
WMKC_OF((wmkcByte *sbox, wmkcByte *rsbox))
{
    for(wmkc_u32 x = 0; x < 256; ++x)
        rsbox[sbox[x]] = x;
}
