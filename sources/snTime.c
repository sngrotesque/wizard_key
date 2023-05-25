#include <snTime.h>

WMKC_PUBLIC(snErr_ctx) snTime_GetNowTime WMKC_OPEN_API
WMKC_OF((wmkcChar **dst))
{
    snErr_ctx error;
    if(!dst) {
        snErr_return(error, snErr_ErrNULL, "snTime_GetNowTime: dst is NULL.");
    }
    wmkc_u32 dst_size = 24;
    if(!wmkcMemoryNew(wmkcChar *, (*dst), dst_size)) {
        snErr_return(error, snErr_ErrMemory,
            "snTime_GetNowTime: (*dst) failed to apply for memory.");
    }
    time_t t;
    struct tm *tp;

    wmkcMemoryZero((*dst), dst_size);
    time(&t);
    tp = localtime(&t);

    strftime((*dst), dst_size, "%Y-%m-%d %H:%M:%S", tp);

    snErr_return(error, snErr_OK, "OK.");
}

// WMKC_PUBLIC(wmkcFloat) snTime_GetNowSec WMKC_OPEN_API
// WMKC_OF((wmkcVoid))
// {
//     wmkcFloat elapsedTime = 0;
// #   if defined(_WIN32)
//     LARGE_INTEGER frequency, timer;
//     QueryPerformanceFrequency(&frequency);
//     QueryPerformanceCounter(&timer);
//     elapsedTime = (timer.QuadPart) * 1000000.0 / frequency.QuadPart;
// #   endif

//     return elapsedTime;
// }

WMKC_PUBLIC(wmkcVoid) snTime_TimerBegin WMKC_OPEN_API
WMKC_OF((snTime_ctx *timer))
{
#if defined(__linux)
    gettimeofday(&timer->start, wmkcNull);
#elif defined(_WIN32) // #if defined(__linux)
    timer->start = GetTickCount64();
#endif
}

WMKC_PUBLIC(wmkcVoid) snTime_TimerEnd WMKC_OPEN_API
WMKC_OF((snTime_ctx *timer))
{
#if defined(__linux)
    gettimeofday(&timer->stop, wmkcNull);
    timer->totalTime = (wmkcFloat)(CLOCKS_PER_SEC *
        (timer->stop.tv_sec - timer->start.tv_sec) +
        timer->stop.tv_usec - timer->start.tv_usec) /
        CLOCKS_PER_SEC;
#elif defined(_WIN32)
    timer->totalTime = (wmkcFloat)(((wmkcFloat)GetTickCount64() -
        (wmkcFloat)timer->start) / CLOCKS_PER_SEC);
#endif
}

WMKC_PUBLIC(wmkcVoid) snTime_TimerPrint WMKC_OPEN_API
WMKC_OF((wmkcString text, snTime_ctx *timer))
{
    printf("%s%.2lfs.\n", text, timer->totalTime);
}
