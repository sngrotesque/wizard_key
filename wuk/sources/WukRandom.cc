#include <WukRandom.hh>

wuk::Random::Random()
{
    this->generator = std::mt19937(rd());
}

wSize wuk::Random::rand()
{
    return this->randint(0, ~0);
}

wSize wuk::Random::randint(wSize min, wSize max)
{
    std::uniform_int_distribution<> dis(min, max);
    return dis(this->generator);
}

void wuk::Random::urandom(wByte *buffer, wSize length)
{
    if(!buffer || !length) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::Random::urandom",
            "buffer or length is NULL.");
    }

#   if defined(WUK_PLATFORM_WINOS)
    NTSTATUS status = BCryptGenRandom(nullptr, buffer, length, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
    if (status != STATUS_SUCCESS) {
        throw wuk::Exception(status, "wuk::Random::urandom",
            "BCryptGenRandom function returned an error code when called.");
    }
#   elif defined(WUK_PLATFORM_LINUX)
    if(getrandom(buffer, length, GRND_RANDOM) == EOF) {
        throw wuk::Exception(static_cast<wuk::Error>(errno), "wuk::Random::urandom",
            "getrandom function returned an error code when called.");
    }
#   endif
}

std::string wuk::Random::urandom(wU32 length)
{
    if(!length) {
        return std::string();
    }

    wByte *buffer = wuk::m_alloc<wByte *>(length);
    if(!buffer) {
        throw wuk::Exception(wuk::Error::MEMORY, "wuk::Random::urandom",
            "Failed to allocate memory for buffer.");
    }

    this->urandom(buffer, length);

    std::string result(reinterpret_cast<char *>(buffer), length);
    wuk::m_free(buffer);

    return result;
}
