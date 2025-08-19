#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <random>

namespace wuk {
    class LIBWUK_API Random {
    private:
        std::mt19937 generator;

    public:
        Random() = default;

    public:
        wSize rand();
        wSize randint(wSize min, wSize max);

        void bytes(wByte *buffer, wSize length);
        std::string bytes(wU32 length);
    };
}

#endif /* WUK_SUPPORT */
