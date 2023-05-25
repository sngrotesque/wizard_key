#ifndef __SN_RANDOM__
#define __SN_RANDOM__

#include <snConf.h>
#include <time.h>

#if defined(__SN_MEMORY)
#   include <stdlib.h>
#endif

#if defined(__linux)
#   include <sys/random.h>
#elif defined(_WIN32)
#   include <windows.h>
#   define EPOCHFILETIME   (116444736000000000ULL)
#endif

// 设置时间随机数种子
WMKC_PUBLIC(wmkcVoid) snRand_Seed WMKC_OPEN_API
WMKC_OF((wmkcVoid));

// 随机数生成
WMKC_PUBLIC(wmkcSize) snRand_Rand WMKC_OPEN_API
WMKC_OF((wmkcVoid));

// 范围随机数生成
WMKC_PUBLIC(wmkcSize) snRand_Randint WMKC_OPEN_API
WMKC_OF((wmkcSize min, wmkcSize max));

// 安全随机数生成
WMKC_PUBLIC(wmkcVoid) snRand_Urandom WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkc_u32 size));

#endif // #ifndef __SN_RANDOM__
