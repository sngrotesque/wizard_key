#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <random>

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
