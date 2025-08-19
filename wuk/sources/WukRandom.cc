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

wuk::Random::Random()
{
    std::random_device rd;
    std::seed_seq seed{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
    this->generator = std::mt19937(seed);
}

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
    if(!length) {
        return std::string();
    }

    wByte *buffer = wuk::m_alloc<wByte *>(length);
    if(!buffer) {
        throw wuk::Exception(wuk::Error::MEMORY, "wuk::Random::bytes",
            "Failed to allocate memory for buffer.");
    }

    this->bytes(buffer, length);

    std::string result(reinterpret_cast<char *>(buffer), length);
    wuk::m_free(buffer);

    return result;
}
