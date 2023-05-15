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
SN_PUBLIC(snVoid) snRand_Seed SN_OPEN_API
SN_FUNC_OF((snVoid));

// 随机数生成
SN_PUBLIC(snSize) snRand_Rand SN_OPEN_API
SN_FUNC_OF((snVoid));

// 范围随机数生成
SN_PUBLIC(snSize) snRand_Randint SN_OPEN_API
SN_FUNC_OF((snSize min, snSize max));

// 安全随机数生成
SN_PUBLIC(snVoid) snRand_Urandom SN_OPEN_API
SN_FUNC_OF((snByte *buf, sn_u32 size));

#endif // #ifndef __SN_RANDOM__
