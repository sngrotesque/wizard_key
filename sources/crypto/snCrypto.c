#include <crypto/snCrypto.h>

SN_PUBLIC(snVoid) snCrypto_sbox_init SN_OPEN_API
SN_FUNC_OF((snByte *sbox))
{
    snRand_Seed();
    static sn_u32 r, x, left, right;
    static snByte swap;

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

SN_PUBLIC(snVoid) snCrypto_rsbox_init SN_OPEN_API
SN_FUNC_OF((snByte *sbox, snByte *rsbox))
{
    for(sn_u32 x = 0; x < 256; ++x)
        rsbox[sbox[x]] = x;
}
