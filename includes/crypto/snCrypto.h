#ifndef __SN_CRYPTO__ // snCrypto
#define __SN_CRYPTO__
#include <snConf.h>
#include <snRand.h>

// 生成sbox
WMKC_PUBLIC(wmkcVoid) snCrypto_sbox_init WMKC_OPEN_API
WMKC_OF((wmkcByte *sbox));

// 生成与sbox对应的rsbox
WMKC_PUBLIC(wmkcVoid) snCrypto_rsbox_init WMKC_OPEN_API
WMKC_OF((wmkcByte *sbox, wmkcByte *rsbox));

#endif // snCrypto