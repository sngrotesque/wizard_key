#ifndef WMKC_BINASCII
#define WMKC_BINASCII

#include <wmkc_conf.h>
#if !defined(WMKC_MEMORY)
#include <malloc.h>
#endif

// 用来将二进制数据转为HEX字符串："123abc" -> 313233616263
WMKC_PUBLIC(wmkcErr_obj) wmkcBinascii_b2a_hex WMKC_OPEN_API
WMKC_OF((wmkcByte **dst, wmkcByte *src, wmkcSize srcSize));

// 用来将HEX字符串转为二进制数据：313233616263 -> "123abc"
WMKC_PUBLIC(wmkcErr_obj) wmkcBinascii_a2b_hex WMKC_OPEN_API
WMKC_OF((wmkcByte **dst, wmkcByte *src, wmkcSize srcSize));

#endif
