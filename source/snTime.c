#include <snTime.h>

SN_PUBLIC(snChar *) snTime_GetNowTime SN_OPEN_API
SN_FUNC_OF((snVoid))
{
    static snChar timetimetime[32];
    static snChar *timePtr = timetimetime;
    static time_t timer;

#if defined(__linux)
    static struct tm* tblock;
    timer = time(snNull);
    tblock = localtime(&timer);
    snprintf(timetimetime, sizeof(timetimetime),
        "%04d-%02d-%02d %02d:%02d:%02d",
        tblock->tm_year + 1900,
        tblock->tm_mon + 1,
        tblock->tm_mday,
        tblock->tm_hour,
        tblock->tm_min,
        tblock->tm_sec);
#elif defined(_WIN32)
    static struct tm tblock;
    time(&timer);
    localtime_s(&tblock, &timer);
    sprintf_s(timetimetime, sizeof(timetimetime),
        "%04"PRId32"-%02"PRId32"-%02"PRId32" %02"PRId32":%02"PRId32":%02"PRId32,
        tblock.tm_year + 1900,
        tblock.tm_mon + 1,
        tblock.tm_mday,
        tblock.tm_hour,
        tblock.tm_min,
        tblock.tm_sec);
#endif // #if defined(__linux)

    return timePtr;
}

SN_PUBLIC(snFloat) snTime_GetNowSec SN_OPEN_API
SN_FUNC_OF((snVoid))
{
    snFloat elapsedTime;
#   if defined(_WIN32)
    LARGE_INTEGER frequency, timer;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&timer);
    elapsedTime = (timer.QuadPart) * 1000000.0 / frequency.QuadPart;
#   endif

    return elapsedTime;
}

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
SN_FUNC_OF((const snChar *text, snTime_ctx *timer))
{
    printf("%s%.2lfs.\n", text, timer->totalTime);
}
