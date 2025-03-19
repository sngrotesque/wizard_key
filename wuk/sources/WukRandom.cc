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
    // 后续请改为`wuk/sources/network/WukException.cc`中那样的获取错误字符串
#       ifdef WUK_CRYPTO_RANDOM_OLD
    HCRYPTPROV hProv;
    if(!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
        throw wuk::Exception(static_cast<wuk::Error>(GetLastError()), "wuk::Random::urandom",
            "CryptAcquireContext function returned an error code when called.");
    }
    if(!CryptGenRandom(hProv, length, buffer)) {
        throw wuk::Exception(static_cast<wuk::Error>(GetLastError()), "wuk::Random::urandom",
            "CryptGenRandom function returned an error code when called.");
    }
    if(!CryptReleaseContext(hProv, 0)) {
        throw wuk::Exception(static_cast<wuk::Error>(GetLastError()), "wuk::Random::urandom",
            "CryptReleaseContext function returned an error code when called.");
    }
#       else
    NTSTATUS status = BCryptGenRandom(nullptr, buffer, length, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
    if (status != STATUS_SUCCESS) {
        throw wuk::Exception(status, "wuk::Random::urandom",
            "BCryptGenRandom function returned an error code when called.");
    }
#       endif
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
