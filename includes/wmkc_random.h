#ifndef WMKC_RANDOM
#define WMKC_RANDOM

#include <wmkc_conf.h>
#include <time.h>

#if defined(WMKC_MEMORY)
#   include <stdlib.h>
#endif

#if defined(WMKC_LINUX_SUPPORT)
#   include <sys/random.h>
#elif defined(WMKC_WINDOWS_SUPPORT)
#   include <windows.h>
#   define EPOCHFILETIME   (116444736000000000ULL)
#endif

// 设置时间随机数种子
WMKC_PUBLIC(wmkcVoid) wmkcRandom_seed WMKC_OPEN_API
WMKC_OF((wmkcVoid));

// 随机数生成
WMKC_PUBLIC(wmkcSize) wmkcRandom_rand WMKC_OPEN_API
WMKC_OF((wmkcVoid));

// 范围随机数生成
WMKC_PUBLIC(wmkcSize) wmkcRandom_randint WMKC_OPEN_API
WMKC_OF((wmkcSize min, wmkcSize max));

// 安全随机数生成
WMKC_PUBLIC(wmkcVoid) wmkcRandom_urandom WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkc_u32 size));

#endif
