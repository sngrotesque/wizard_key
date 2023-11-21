#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_TIME
#define WMKC_CPP_TIME
#include <wmkc_exception.hpp>
#include <sys/timeb.h>
#include <time.h>

#ifdef WMKC_PLATFORM_WINOS
#include <Windows.h>
#elif defined(WMKC_PLATFORM_LINUX)
#include <sys/time.h>
#include <unistd.h>
#endif

class wmkcTime {
    public:
        std::string getNowTime(std::string timeFormat = "%Y-%m-%d %H:%M:%S");
        wmkcVoid sleep(double _t);
        double time();
};

#endif /* WMKC_TIME */
#endif /* WMKC_SUPPORT */
