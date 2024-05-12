#include <random.hpp>

constexpr wByte BIT_BYTE_SWAP(wByte x)
{
    return ((x & 0xf) << 4) ^ (x >> 4);
}

wmkc::random::random()
{
    this->seed();
}

wVoid wmkc::random::seed()
{
    wmkc::Time timer;
    srand((unsigned)(timer.time() * 1.e6));
}

wSize wmkc::random::rand()
{
    wSize n = 0, num[32];
    wU32 x, count;
    for(x = 0; x < 32; ++x) {
        num[x] = ::rand() ^ (::rand() ^ (::rand() & ::rand()));
        n = n + (n ^ num[x]);
    }
    n = n + BIT_BYTE_SWAP(::rand() & 0x0f);
    for(count = 0; count < 32; ++count) {
        for(x = 0; x < 32; ++x) {
            num[x] = ::rand() ^ (::rand() ^ (::rand() & ::rand()));
        }
        n = (n + (n ^ num[count])) ^ count;
        n = (n + ::rand()) ^ BIT_BYTE_SWAP((count + (::rand() ^
                                        (n - num[count]))) & 0xff);
    }
    return n;
}

wSize wmkc::random::randint(wSize min, wSize max)
{
    return this->rand() % (max - min + 1) + min;
}

wVoid wmkc::random::urandom(wByte *buf, wSize size)
{
    if(!buf || !size) {
        wmkc::exception(wmkcErr_ErrNULL, "wmkc::random::urandom",
                                        "buf or size is NULL.");
    }

#   if defined(WMKC_PLATFORM_WINOS)
    HCRYPTPROV hProv;
    if(!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
        wmkc::exception(wmkcErr_ErrSysFunc, "wmkc::random::urandom",
            "CryptAcquireContext function returned an error code when called.");
    }
    if(!CryptGenRandom(hProv, size, buf)) {
        wmkc::exception(wmkcErr_ErrSysFunc, "wmkc::random::urandom",
            "CryptGenRandom function returned an error code when called.");
    }
    if(!CryptReleaseContext(hProv, 0)) {
        wmkc::exception(wmkcErr_ErrSysFunc, "wmkc::random::urandom",
            "CryptReleaseContext function returned an error code when called.");
    }
#   elif defined(WMKC_PLATFORM_LINUX)
    if(getrandom(buf, size, GRND_RANDOM) == wmkcErr_Err) {
        wmkc::exception(wmkcErr_ErrSysFunc, "wmkc::random::urandom",
            "getrandom function returned an error code when called.");
    }
#   endif
}

std::string wmkc::random::urandom(wU32 size)
{
    if(!size) {
        return std::string();
    }
    wByte *buf = new wByte[size];
    if(!buf) {
        wmkc::exception(wmkcErr_ErrMemory, "wmkc::random::urandom",
            "Failed to allocate memory for buf.");
    }

    try {
        this->urandom(buf, size);
    } catch(std::exception &e) {
        delete[] buf;
        throw e;
    }

    std::string result((wChar *)buf, size);
    delete[] buf;
    return result;
}
