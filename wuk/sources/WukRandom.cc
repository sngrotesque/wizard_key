#include <WukRandom.hh>

constexpr wByte BIT_BYTE_SWAP(wByte i)
{
    return ((i & 0xf) << 4) ^ (i >> 4);
}

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

void wuk::Random::urandom(wByte *buf, wSize size)
{
    if(!buf || !size) {
        throw wuk::Exception(wuk::Error::NPTR, "wuk::Random::urandom",
                                        "buf or size is NULL.");
    }

#   if defined(WUK_PLATFORM_WINOS)
    // 后续请改为`wuk/sources/network/WukException.cc`中那样的获取错误字符串
#   ifdef WUK_CRYPTO_RANDOM_OLD
    HCRYPTPROV hProv;
    if(!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
        throw wuk::Exception(static_cast<wuk::Error>(GetLastError()), "wuk::Random::urandom",
            "CryptAcquireContext function returned an error code when called.");
    }
    if(!CryptGenRandom(hProv, size, buf)) {
        throw wuk::Exception(static_cast<wuk::Error>(GetLastError()), "wuk::Random::urandom",
            "CryptGenRandom function returned an error code when called.");
    }
    if(!CryptReleaseContext(hProv, 0)) {
        throw wuk::Exception(static_cast<wuk::Error>(GetLastError()), "wuk::Random::urandom",
            "CryptReleaseContext function returned an error code when called.");
    }
#   else
    NTSTATUS status = BCryptGenRandom(nullptr, buf, size, 2);
    if (status != STATUS_SUCCESS) {
        throw wuk::Exception(status, "wuk::Random::urandom",
            "BCryptGenRandom function returned an error code when called.");
    }
#   endif
#   elif defined(WUK_PLATFORM_LINUX)
    if(getrandom(buf, size, GRND_RANDOM) == EOF) {
        throw wuk::Exception(static_cast<wuk::Error>(errno), "wuk::Random::urandom",
            "getrandom function returned an error code when called.");
    }
#   endif
}

std::string wuk::Random::urandom(wU32 size)
{
    if(!size) {
        return std::string();
    }

    wByte *buffer = wuk::m_alloc<wByte *>(size);
    if(!buffer) {
        throw wuk::Exception(wuk::Error::MEMORY, "wuk::Random::urandom",
            "Failed to allocate memory for buffer.");
    }

    this->urandom(buffer, size);

    std::string result(reinterpret_cast<char *>(buffer), size);
    wuk::m_free(buffer);

    return result;
}
