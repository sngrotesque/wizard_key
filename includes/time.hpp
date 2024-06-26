#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_TIME
#define WMKC_CPP_TIME
#include <config/exception.hpp>
#include <sys/timeb.h>
#include <time.h>

#ifdef WMKC_PLATFORM_WINOS
#include <Windows.h>
#elif defined(WMKC_PLATFORM_LINUX)
#include <sys/time.h>
#include <unistd.h>
#endif

namespace wmkc {
    class LIBWMKC_API Time {
    public:
        Time() {}
        std::string get_now_time(std::string timeFormat = "%a, %m/%d %Y %H:%M:%S");
        void sleep(double _t);
        double time();
    };
}

#endif /* WMKC_TIME */
#endif /* WMKC_SUPPORT */
