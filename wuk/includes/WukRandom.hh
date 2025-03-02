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
#endif

namespace wuk {
    class LIBWUK_API Random {
    private:
        std::random_device rd;
        std::mt19937 generator;

    public:
        Random();

    public:
        wSize rand();
        wSize randint(wSize min, wSize max);
        void urandom(wByte *buf, wSize size);
        std::string urandom(wU32 size);
    };
}

#endif /* WUK_SUPPORT */
