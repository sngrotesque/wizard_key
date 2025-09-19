#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukException.hh>
#include <WukBuffer.hh>
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
        std::mt19937 m_gen;

    public:
        Random()
        {
            std::random_device rd;
            std::seed_seq seed{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
            this->m_gen = std::mt19937(seed);
        }

    public:
        inline wuk::f64 random()
        {
            std::uniform_real_distribution<wuk::f64> dis(0.0, 1.0);
            return dis(this->m_gen);
        }

        inline wuk::f64 uniform(wuk::f64 min_val, wuk::f64 max_val)
        {
            if (min_val > max_val) {
                return 0;
            }
            std::uniform_real_distribution<wuk::f64> dis(min_val, max_val);
            return dis(this->m_gen);
        }

        inline wuk::ulong randint(wuk::ulong min_val, wuk::ulong max_val)
        {
            if (min_val > max_val) {
                return 0;
            }
            std::uniform_int_distribution<wuk::ulong> dis(min_val, max_val);
            return dis(this->m_gen);
        }

        inline void bytes(wuk::byte *buffer, wuk::ulong length)
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

        inline wuk::Buffer bytes(wuk::u32 length)
        {
            if(!length) {
                return {};
            }
            wuk::Buffer result(length, 0);

            this->bytes(result.write(length), length);

            return result;
        }

        template <typename T>
        inline const T &choice(const std::vector<T> &arr)
        {
            return arr.at(this->randint(0, arr.size() - 1));
        }

        template <typename T>
        inline void shuffle(std::vector<T> &array)
        {
            if (array.empty()) {
                return;
            }

            // 使用Fisher-Yates洗牌算法
            for (wuk::ulong i = array.size() - 1; i > 0; --i) {
                wuk::ulong j = this->randint(0, i);

                std::swap(array[i], array[j]);
            }
        }
    };
}

#endif /* WUK_SUPPORT */
