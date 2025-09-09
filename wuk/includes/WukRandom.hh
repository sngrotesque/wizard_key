#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukException.hh>
#include <WukMemory.hh>
#include <random>
#include <vector>

#if defined(WUK_PLATFORM_LINUX)
#   include <sys/random.h>
#   ifdef WUK_PLATFORM_ANDROID
#       include <stdio.h>
#   endif
#elif defined(WUK_PLATFORM_WINOS)
#   include <windows.h>
#   include <bcrypt.h>
#   include <ntstatus.h>
#   ifdef WUK_COMPILER_MSVC
#       pragma comment(lib, "bcrypt")
#   endif
#endif

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
        wuk::ulong rand()
        {
            return this->randint(0, ~0ULL);
        }

        wuk::ulong randint(wuk::ulong min, wuk::ulong max)
        {
            std::uniform_int_distribution<wuk::ulong> dis(min, max);
            return dis(this->generator);
        }

        void bytes(wuk::byte *buffer, wuk::ulong length)
        {
            if(!buffer) {
                throw wuk::Exception(wuk::Error::NPTR, "wuk::Random::bytes",
                    "buffer is nullptr.");
            }

#           if defined(WUK_PLATFORM_WINOS)
            NTSTATUS status = BCryptGenRandom(nullptr, buffer, length,
                                            BCRYPT_USE_SYSTEM_PREFERRED_RNG);
            if (status != STATUS_SUCCESS) {
                throw wuk::Exception(status, "wuk::Random::bytes",
                    "Unable to obtain a random number, BCryptGenRandom error.");
            }
#           elif defined(WUK_PLATFORM_LINUX)
#           ifdef WUK_PLATFORM_ANDROID
            // Termux对getrandom函数的支持性不高，此处的代码是为了兼容它，如果后续不考虑Termux平台的话可移除此代码。
            FILE *fp = fopen("/dev/urandom", "rb");
            if (fp == nullptr) {
                throw wuk::Exception(wuk::Error::FTOFO, "wuk::Random::bytes",
                    "Unable to obtain a random number.");
            }
            if (fread(buffer, 1, length, fp) == 0) {
                fclose(fp);
                throw wuk::Exception(wuk::Error::ERR, "wuk::Random::bytes",
                    "Unable to obtain a random number.");
            }
            fclose(fp);
#           else
            if(getrandom(buffer, length, GRND_RANDOM) == EOF) {
                wuk::i32 err_code = errno;
                throw wuk::Exception(err_code, "wuk::Random::bytes",
                    strerror(err_code));
            }
#           endif
#           endif
        }

        std::string bytes(wuk::u32 length)
        {
            if(!length) {
                return {};
            }
            std::string result(length, '\0');

            this->bytes(reinterpret_cast<wuk::byte *>(result.data()), length);

            return result;
        }

        template <typename T>
        const T &choice(const std::vector<T> &arr)
        {
            try {
                auto i = this->randint(0, arr.size() - 1);
                return arr.at(i);
            } catch (const std::out_of_range &e) {
                throw wuk::Exception(wuk::Error::ERR, "wuk::Random::choice",
                    e.what());
            }
        }
    };
}

#endif /* WUK_SUPPORT */
