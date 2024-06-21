#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_RANDOM
#define WMKC_CPP_RANDOM
#include <config/exception.hpp>
#include <time.hpp>

#if defined(WMKC_PLATFORM_LINUX)
#   include <sys/random.h>
#elif defined(WMKC_PLATFORM_WINOS)
#   include <windows.h>
#endif

namespace wmkc {
    class LIBWMKC_API Random {
    public:
        Random();
        void seed();
        wSize rand();
        wSize randint(wSize min, wSize max);
        void urandom(wByte *buf, wSize size);
        std::string urandom(wU32 size);
    };
}

#endif /* WMKC_CPP_RANDOM */
#endif /* WMKC_SUPPORT */
