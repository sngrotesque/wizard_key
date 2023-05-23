#include <snTime.h>

SN_PUBLIC(snErr_ctx) snTime_GetNowTime SN_OPEN_API
SN_FUNC_OF((snChar **dst))
{
    snErr_ctx error;
    if(!dst) {
        snErr_return(error, snErr_ErrNULL, "snTime_GetNowTime: dst is NULL.");
    }
    sn_u32 dst_size = 24;
    if(!snMemoryNew(snChar *, (*dst), dst_size)) {
        snErr_return(error, snErr_ErrMemory,
            "snTime_GetNowTime: (*dst) failed to apply for memory.");
    }
    time_t t;
    struct tm *tp;

    snMemoryZero((*dst), dst_size);
    time(&t);
    tp = localtime(&t);

    strftime((*dst), dst_size, "%Y-%m-%d %H:%M:%S", tp);

    snErr_return(error, snErr_OK, "OK.");
}

// SN_PUBLIC(snFloat) snTime_GetNowSec SN_OPEN_API
// SN_FUNC_OF((snVoid))
// {
//     snFloat elapsedTime = 0;
// #   if defined(_WIN32)
//     LARGE_INTEGER frequency, timer;
//     QueryPerformanceFrequency(&frequency);
//     QueryPerformanceCounter(&timer);
//     elapsedTime = (timer.QuadPart) * 1000000.0 / frequency.QuadPart;
// #   endif

//     return elapsedTime;
// }

SN_PUBLIC(snVoid) snTime_TimerBegin SN_OPEN_API
SN_FUNC_OF((snTime_ctx *timer))
{
#if defined(__linux)
    gettimeofday(&timer->start, snNull);
#elif defined(_WIN32) // #if defined(__linux)
    timer->start = GetTickCount64();
#endif
}

SN_PUBLIC(snVoid) snTime_TimerEnd SN_OPEN_API
SN_FUNC_OF((snTime_ctx *timer))
{
#if defined(__linux)
    gettimeofday(&timer->stop, snNull);
    timer->totalTime = (snFloat)(CLOCKS_PER_SEC *
        (timer->stop.tv_sec - timer->start.tv_sec) +
        timer->stop.tv_usec - timer->start.tv_usec) /
        CLOCKS_PER_SEC;
#elif defined(_WIN32)
    timer->totalTime = (snFloat)(((snFloat)GetTickCount64() -
        (snFloat)timer->start) / CLOCKS_PER_SEC);
#endif
}

SN_PUBLIC(snVoid) snTime_TimerPrint SN_OPEN_API
SN_FUNC_OF((snString text, snTime_ctx *timer))
{
    printf("%s%.2lfs.\n", text, timer->totalTime);
}
