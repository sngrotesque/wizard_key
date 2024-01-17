#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_RANDOM
#define WMKC_CPP_RANDOM
#include <wmkc_exception.hpp>
#include <wmkc_time.hpp>
#include <wmkc_basic.hpp>

#if defined(WMKC_PLATFORM_LINUX)
#   include <sys/random.h>
#elif defined(WMKC_PLATFORM_WINOS)
#   include <windows.h>
#endif

class wmkcRandom {
    public:
        wmkcRandom();
        wmkcVoid seed();
        wmkcSize rand();
        wmkcSize randint(wmkcSize min, wmkcSize max);
        wmkcVoid urandom(wmkcByte *buf, wmkcSize size);
        std::string urandom(wmkc_u32 size);
};

#endif /* WMKC_CPP_RANDOM */
#endif /* WMKC_SUPPORT */
