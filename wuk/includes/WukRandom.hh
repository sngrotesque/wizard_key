#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_RANDOM
#define WUK_CPP_RANDOM
#include <config/WukException.hh>
#include <WukTime.hh>

#if defined(WUK_PLATFORM_LINUX)
#   include <sys/random.h>
#elif defined(WUK_PLATFORM_WINOS)
#   include <windows.h>
#endif

namespace wuk {
    class LIBWUK_API Random {
    public:
        Random();
        void seed();
        wSize rand();
        wSize randint(wSize min, wSize max);
        void urandom(wByte *buf, wSize size);
        std::string urandom(wU32 size);
    };
}

#endif /* WUK_CPP_RANDOM */
#endif /* WUK_SUPPORT */
