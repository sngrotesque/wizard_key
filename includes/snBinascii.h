#ifndef __SN_BINASCII__
#define __SN_BINASCII__

#include <snConf.h>
#if !defined(__SN_MEMORY)
#include <malloc.h>
#endif

// 用来将二进制数据转为HEX字符串："123abc" -> 313233616263
SN_PUBLIC(snByte *) snBinascii_b2a_hex SN_OPEN_API
SN_FUNC_OF((snByte *src, snSize srcSize));

// 用来将HEX字符串转为二进制数据：313233616263 -> "123abc"
SN_PUBLIC(snByte *) snBinascii_a2b_hex SN_OPEN_API
SN_FUNC_OF((snByte *src, snSize srcSize));

#endif
