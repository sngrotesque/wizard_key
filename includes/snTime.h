#ifndef __SN_TIME__
#define __SN_TIME__

#include <snConf.h>

#include <time.h>
#include <sys/timeb.h>

#ifdef _WIN32
#include <windows.h>
#endif

#if defined(__linux)
#include <sys/time.h>
#endif

typedef struct {
#if defined(__linux)
    struct timeval start;
    struct timeval stop;
#elif defined(_WIN32)
    ULONGLONG start;
    ULONGLONG stop;
#endif
    snFloat totalTime;
} snTime_ctx;

// 获取当前时间。格式：年月日-时分秒
SN_PUBLIC(snChar *) snTime_GetNowTime SN_OPEN_API
SN_FUNC_OF((snVoid));

// 获取当前的毫秒
SN_PUBLIC(snFloat) snTime_GetNowSec SN_OPEN_API
SN_FUNC_OF((snVoid));

// 开始计时
SN_PUBLIC(snVoid) snTime_TimerBegin SN_OPEN_API
SN_FUNC_OF((snTime_ctx *timer));

// 结束计时
SN_PUBLIC(snVoid) snTime_TimerEnd SN_OPEN_API
SN_FUNC_OF((snTime_ctx *timer));

// 打印计时后的结果
SN_PUBLIC(snVoid) snTime_TimerPrint SN_OPEN_API
SN_FUNC_OF((const snChar *text, snTime_ctx *timer));

#endif
