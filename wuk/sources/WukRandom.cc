#include <WukRandom.hh>

#include <config/WukException.hh>
#include <WukMemory.hh>

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

wSize wuk::Random::rand()
{
    return this->randint(0, ~0ULL);
}

wSize wuk::Random::randint(wSize min, wSize max)
{
    std::uniform_int_distribution<wSize> dis(min, max);
    return dis(this->generator);
}

void wuk::Random::bytes(wByte *buffer, wSize length)
{
    if(!buffer || !length) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::Random::bytes",
            "buffer or length is NULL.");
    }

#   if defined(WUK_PLATFORM_WINOS)
    NTSTATUS status = BCryptGenRandom(nullptr, buffer, length,
                                    BCRYPT_USE_SYSTEM_PREFERRED_RNG);
    if (status != STATUS_SUCCESS) {
        throw wuk::Exception(status, "wuk::Random::bytes",
            "BCryptGenRandom function returned an error code when called.");
    }
#   elif defined(WUK_PLATFORM_LINUX)
    if(getrandom(buffer, length, GRND_RANDOM) == EOF) {
        throw wuk::Exception(static_cast<wuk::Error>(errno), "wuk::Random::bytes",
            "getrandom function returned an error code when called.");
    }
#   endif
}

std::string wuk::Random::bytes(wU32 length)
{
    if(!length) return {};

    std::string result(length, '\0');

    this->bytes(reinterpret_cast<wuk::byte *>(result.data()), length);

    return result;
}
