#ifndef WMKC_CRYPTO
#define WMKC_CRYPTO
#include <wmkc_conf.h>
#include <wmkc_random.h>

// 生成sbox
WMKC_PUBLIC(wmkcVoid) wmkcCrypto_sbox_init WMKC_OPEN_API
WMKC_OF((wmkcByte *sbox));

// 生成与sbox对应的rsbox
WMKC_PUBLIC(wmkcVoid) wmkcCrypto_rsbox_init WMKC_OPEN_API
WMKC_OF((wmkcByte *sbox, wmkcByte *rsbox));

#endif // snCrypto