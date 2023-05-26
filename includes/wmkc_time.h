#ifndef WMKC_TIME
#define WMKC_TIME

#include <wmkc_conf.h>

#include <time.h>
#include <sys/timeb.h>

#ifdef WMKC_PLATFORM_WINOS
#include <windows.h>
#endif

#if defined(WMKC_PLATFORM_LINUX)
#include <sys/time.h>
#endif

typedef struct {
#if defined(WMKC_PLATFORM_LINUX)
    struct timeval start;
    struct timeval stop;
#elif defined(WMKC_PLATFORM_WINOS)
    ULONGLONG start;
    ULONGLONG stop;
#endif
    wmkcFloat totalTime;
} wmkcTime_ctx;

// 获取当前时间。格式：年月日-时分秒
WMKC_PUBLIC(wmkcErr_ctx) wmkcTime_GetNowTime WMKC_OPEN_API
WMKC_OF((wmkcChar **dst));

// 开始计时
WMKC_PUBLIC(wmkcVoid) wmkcTime_TimerBegin WMKC_OPEN_API
WMKC_OF((wmkcTime_ctx *timer));

// 结束计时
WMKC_PUBLIC(wmkcVoid) wmkcTime_TimerEnd WMKC_OPEN_API
WMKC_OF((wmkcTime_ctx *timer));

// 打印计时后的结果
WMKC_PUBLIC(wmkcVoid) wmkcTime_TimerPrint WMKC_OPEN_API
WMKC_OF((wmkcString text, wmkcTime_ctx *timer));

#endif
