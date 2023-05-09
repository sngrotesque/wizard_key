#ifndef __SN_CRYPTO__ // snCrypto
#define __SN_CRYPTO__
#include <snConf.h>
#include <snRand.h>

// 生成sbox
SN_PUBLIC(snVoid) snCrypto_sbox_init SN_OPEN_API
SN_FUNC_OF((snByte *sbox));

// 生成与sbox对应的rsbox
SN_PUBLIC(snVoid) snCrypto_rsbox_init SN_OPEN_API
SN_FUNC_OF((snByte *sbox, snByte *rsbox));

#endif // snCrypto