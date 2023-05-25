#ifndef __SN_BINASCII__
#define __SN_BINASCII__

#include <snConf.h>
#if !defined(__SN_MEMORY)
#include <malloc.h>
#endif

// 用来将二进制数据转为HEX字符串："123abc" -> 313233616263
WMKC_PUBLIC(snErr_ctx) snBinascii_b2a_hex WMKC_OPEN_API
WMKC_OF((wmkcByte **dst, wmkcByte *src, wmkcSize srcSize));

// 用来将HEX字符串转为二进制数据：313233616263 -> "123abc"
WMKC_PUBLIC(snErr_ctx) snBinascii_a2b_hex WMKC_OPEN_API
WMKC_OF((wmkcByte **dst, wmkcByte *src, wmkcSize srcSize));

#endif
