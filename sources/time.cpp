#include <time.hpp>

std::string wmkc::Time::get_now_time(std::string timeFormat)
{
    char resultString[66]{};
    time_t tm_val;

    ::time(&tm_val);
#   if defined(WMKC_PLATFORM_WINOS) && defined(_MSC_VER)
    struct tm *tm_p = new struct tm;
    localtime_s(tm_p, &tm_val);
#   else
    struct tm *tm_p = localtime(&tm_val);
#   endif

    strftime(resultString, sizeof(resultString), timeFormat.c_str(), tm_p);

    return std::string(resultString);
}

void wmkc::Time::sleep(double _t)
{
#   if defined(WMKC_PLATFORM_WINOS)
    Sleep(_t * 1e3);
#   elif defined(WMKC_PLATFORM_LINUX)
    usleep(_t * 1e6);
#   endif
}

double wmkc::Time::time()
{
#   if defined(WMKC_PLATFORM_WINOS)
    FILETIME ft;
    LARGE_INTEGER li{};
    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    return ((li.QuadPart - 116444736e9) / 10) / 1.e6;
#   elif defined(WMKC_PLATFORM_LINUX)
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1e6 + tv.tv_usec) / 1.e6;
#   endif
}
