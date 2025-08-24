#pragma once
#include <core/WukConfig.hh>

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
        wuk::ulong rand();
        wuk::ulong randint(wuk::ulong min, wuk::ulong max);

        void bytes(wuk::byte *buffer, wuk::ulong length);
        std::string bytes(wuk::u32 length);
    };
}

#endif /* WUK_SUPPORT */
