#include <wmkc_time.hpp>

std::string wmkcTime::getNowTime(std::string timeFormat)
{
    struct tm *tp = wmkcNull;
    time_t t;

    wmkcChar tmp[32] = {0};

    ::time(&t);
    tp = localtime(&t);

    strftime(tmp, sizeof(tmp), timeFormat.c_str(), tp);

    return std::string(tmp, strlen(tmp));
}

wmkcVoid wmkcTime::sleep(double _t)
{
#   if defined(WMKC_PLATFORM_WINOS)
    Sleep(_t * 1000);
#   elif defined(WMKC_PLATFORM_LINUX)
    usleep(_t * 1000000);
#   endif
}

double wmkcTime::time()
{
#   if defined(WMKC_PLATFORM_WINOS)
    FILETIME ft;
    LARGE_INTEGER li;
    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    return ((li.QuadPart - 116444736000000000ULL) / 10) / 1.e6;
#   elif defined(WMKC_PLATFORM_LINUX)
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000000 + tv.tv_usec) / 1.e6;
#   endif
}
