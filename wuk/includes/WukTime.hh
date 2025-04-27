#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <WukMemory.hh>
#include <chrono>

#ifdef WUK_PLATFORM_WINOS
#include <Windows.h>
#elif defined(WUK_PLATFORM_LINUX)
#include <unistd.h>
#endif

namespace wuk {
    class LIBWUK_API Time {
    public:
        Time() = default;
    
    public:
        void sleep(double delay);
        double time();
    };
}

#endif /* WUK_SUPPORT */
