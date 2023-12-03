#include <wmkc_random.hpp>

wmkcRandom::wmkcRandom()
{
    this->seed();
}

wmkcVoid wmkcRandom::seed()
{
    wmkcTime timer;
    srand((unsigned)(timer.time() * 1.e6));
}

wmkcSize wmkcRandom::rand()
{
    wmkcSize n = 0, num[32];
    wmkc_u32 x, count;
    for(x = 0; x < 32; ++x) {
        num[x] = ::rand() ^ (::rand() ^ (::rand() & ::rand()));
        n = n + (n ^ num[x]);
    }
    n = n + wmkcBasic_byteSwap(::rand() & 0x0f);
    for(count = 0; count < 32; ++count) {
        for(x = 0; x < 32; ++x) {
            num[x] = ::rand() ^ (::rand() ^ (::rand() & ::rand()));
        }
        n = (n + (n ^ num[count])) ^ count;
        n = (n + ::rand()) ^ wmkcBasic_byteSwap((count + (::rand() ^ (n - num[count]))) & 0xff);
    }
    return n;
}

wmkcSize wmkcRandom::randint(wmkcSize min, wmkcSize max)
{
    return this->rand() % (max - min + 1) + min;
}

wmkcVoid wmkcRandom::urandom(wmkcByte *buf, wmkcSize size)
{
    if(!buf || !size) {
        wmkcErr_exception(wmkcErr_ErrNULL, "wmkcRandom::urandom", "buf or size is NULL.");
    }

#   if defined(WMKC_PLATFORM_WINOS)
    HCRYPTPROV hProv;
    if(!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
        wmkcErr_exception(wmkcErr_ErrSysFunc, "wmkcRandom::urandom",
            "CryptAcquireContext function returned an error code when called.");
    }
    if(!CryptGenRandom(hProv, size, buf)) {
        wmkcErr_exception(wmkcErr_ErrSysFunc, "wmkcRandom::urandom",
            "CryptGenRandom function returned an error code when called.");
    }
    if(!CryptReleaseContext(hProv, 0)) {
        wmkcErr_exception(wmkcErr_ErrSysFunc, "wmkcRandom::urandom",
            "CryptReleaseContext function returned an error code when called.");
    }
#   elif defined(WMKC_PLATFORM_LINUX)
    if(getrandom(buf, size, GRND_RANDOM) == wmkcErr_Err32) {
        wmkcErr_exception(wmkcErr_ErrSysFunc, "wmkcRandom::urandom",
            "getrandom function returned an error code when called.");
    }
#   endif
}

std::string wmkcRandom::urandom(wmkc_u32 size)
{
    if(!size) {
        return std::string();
    }
    wmkcByte *buf = new wmkcByte[size];
    if(!buf) {
        wmkcErr_exception(wmkcErr_ErrMemory, "wmkcRandom::urandom",
            "Failed to allocate memory for buf.");
    }

    try {
        this->urandom(buf, size);
    } catch(std::exception &e) {
        delete[] buf;
        std::rethrow_exception(std::current_exception());
    }

    std::string result((wmkcChar *)buf, size);
    delete[] buf;
    return result;
}
