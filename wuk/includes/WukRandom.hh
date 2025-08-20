#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <random>

namespace wuk {
    class LIBWUK_API Random {
    private:
        std::mt19937 generator;

    public:
        Random()
        {
            std::random_device rd;
            std::seed_seq seed{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
            this->generator = std::mt19937(seed);
        }

    public:
        wSize rand();
        wSize randint(wSize min, wSize max);

        void bytes(wByte *buffer, wSize length);
        std::string bytes(wU32 length);
    };
}

#endif /* WUK_SUPPORT */
