#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <WukMemory.hh>
#include <random>

#if defined(WUK_PLATFORM_LINUX)
#   include <sys/random.h>
#elif defined(WUK_PLATFORM_WINOS)
#   include <windows.h>
#   include <bcrypt.h>
#   include <ntstatus.h>
#   ifdef WUK_COMPILER_MSVC
#       pragma comment(lib, "bcrypt")
#   endif
#endif

namespace wuk {
    class LIBWUK_API WukRandom {
    private:
        std::mt19937 generator;

    public:
        WukRandom();

    public:
        wSize rand();
        wSize randint(wSize min, wSize max);
        void urandom(wByte *buffer, wSize length);
        std::string urandom(wU32 length);
    };
}

#endif /* WUK_SUPPORT */
