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
    wmkcFloat totalTime;
} snTime_ctx;

// 获取当前时间。格式：年月日-时分秒
WMKC_PUBLIC(snErr_ctx) snTime_GetNowTime WMKC_OPEN_API
WMKC_OF((wmkcChar **dst));

// 开始计时
WMKC_PUBLIC(wmkcVoid) snTime_TimerBegin WMKC_OPEN_API
WMKC_OF((snTime_ctx *timer));

// 结束计时
WMKC_PUBLIC(wmkcVoid) snTime_TimerEnd WMKC_OPEN_API
WMKC_OF((snTime_ctx *timer));

// 打印计时后的结果
WMKC_PUBLIC(wmkcVoid) snTime_TimerPrint WMKC_OPEN_API
WMKC_OF((wmkcString text, snTime_ctx *timer));

#endif
