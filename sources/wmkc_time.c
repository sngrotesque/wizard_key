#include <wmkc_time.h>

WMKC_PUBLIC(wmkcErr_ctx) wmkcTime_GetNowTime WMKC_OPEN_API
WMKC_OF((wmkcChar **dst))
{
    wmkcErr_ctx error;
    if(!dst) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcTime_GetNowTime: dst is NULL.");
    }
    wmkc_u32 dst_size = 24;
    if(!wmkcMemoryNew(wmkcChar *, (*dst), dst_size)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcTime_GetNowTime: (*dst) failed to apply for memory.");
    }
    time_t t;
    struct tm *tp;

    wmkcMemoryZero((*dst), dst_size);
    time(&t);
    tp = localtime(&t);

    strftime((*dst), dst_size, "%Y-%m-%d %H:%M:%S", tp);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

// WMKC_PUBLIC(wmkcFloat) wmkcTime_GetNowSec WMKC_OPEN_API
// WMKC_OF((wmkcVoid))
// {
//     wmkcFloat elapsedTime = 0;
// #   if defined(WMKC_PLATFORM_WINOS)
//     LARGE_INTEGER frequency, timer;
//     QueryPerformanceFrequency(&frequency);
//     QueryPerformanceCounter(&timer);
//     elapsedTime = (timer.QuadPart) * 1000000.0 / frequency.QuadPart;
// #   endif

//     return elapsedTime;
// }

WMKC_PUBLIC(wmkcVoid) wmkcTime_TimerBegin WMKC_OPEN_API
WMKC_OF((wmkcTime_ctx *timer))
{
#if defined(WMKC_PLATFORM_LINUX)
    gettimeofday(&timer->start, wmkcNull);
#elif defined(WMKC_PLATFORM_WINOS) // #if defined(WMKC_PLATFORM_LINUX)
    timer->start = GetTickCount64();
#endif
}

WMKC_PUBLIC(wmkcVoid) wmkcTime_TimerEnd WMKC_OPEN_API
WMKC_OF((wmkcTime_ctx *timer))
{
#if defined(WMKC_PLATFORM_LINUX)
    gettimeofday(&timer->stop, wmkcNull);
    timer->totalTime = (wmkcFloat)(CLOCKS_PER_SEC *
        (timer->stop.tv_sec - timer->start.tv_sec) +
        timer->stop.tv_usec - timer->start.tv_usec) /
        CLOCKS_PER_SEC;
#elif defined(WMKC_PLATFORM_WINOS)
    timer->totalTime = (wmkcFloat)(((wmkcFloat)GetTickCount64() -
        (wmkcFloat)timer->start) / CLOCKS_PER_SEC);
#endif
}

WMKC_PUBLIC(wmkcVoid) wmkcTime_TimerPrint WMKC_OPEN_API
WMKC_OF((wmkcString text, wmkcTime_ctx *timer))
{
    printf("%s%.2lfs.\n", text, timer->totalTime);
}
