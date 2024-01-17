#include <wmkc_time.hpp>

std::string wmkcTime::getNowTime(std::string timeFormat)
{
    wmkcChar time_string[66] = {0};
    struct tm *time_ptr = wmkcNull;
    time_t time_var;

    ::time(&time_var);
    time_ptr = localtime(&time_var);

    strftime(time_string, sizeof(time_string), timeFormat.c_str(), time_ptr);

    return std::string(time_string);
}

wmkcVoid wmkcTime::sleep(double _t)
{
#   if defined(WMKC_PLATFORM_WINOS)
    Sleep(_t * 1e3);
#   elif defined(WMKC_PLATFORM_LINUX)
    usleep(_t * 1e6);
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
    return ((li.QuadPart - 116444736e9) / 10) / 1.e6;
#   elif defined(WMKC_PLATFORM_LINUX)
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1e6 + tv.tv_usec) / 1.e6;
#   endif
}
