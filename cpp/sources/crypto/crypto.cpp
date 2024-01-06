#include <crypto/crypto.hpp>

wmkcVoid wmkcCrypto::generate_sbox(wmkcByte *sbox)
{
    wmkc_u32 round, x;
    wmkcByte swap;
    wmkcByte array[2];
    wmkcRandom random;

    for(x = 0; x < 256; ++x)
        sbox[x] = x;

    for(round = 8; round; --round) {
        for(x = 256; x; --x) {
            do {
                random.urandom(array, 2);
            } while(array[0] == array[1]);
            swap = sbox[array[0]];
            sbox[array[0]] = sbox[array[1]];
            sbox[array[1]] = swap;
        }
    }
}

wmkcVoid wmkcCrypto::generate_rsbox(wmkcByte *rsbox, wmkcByte *sbox)
{
    for(wmkc_u32 x = 0; x < 256; ++x) rsbox[sbox[x]] = x;
}
