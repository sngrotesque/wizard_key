#include <wmkc_random.h>

WMKC_PUBLIC(wmkcVoid) wmkcRandom_seed WMKC_OPEN_API
WMKC_OF((wmkcVoid))
{
#   if defined(WMKC_LINUX_SUPPORT)
    clockid_t a = 0;
    struct timespec p;
    clock_gettime(a, &p);
    srand((unsigned)(p.tv_nsec));
#   elif defined(WMKC_WINDOWS_SUPPORT)
    FILETIME ft;
    LARGE_INTEGER li;
    int64_t tt = 0;
    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    // 从1970年1月1日0:0:0:000到现在的微秒数(UTC时间)
    tt = (li.QuadPart - EPOCHFILETIME) / 10;
    srand((unsigned)(tt));
#   endif
}

WMKC_PUBLIC(wmkcSize) wmkcRandom_rand WMKC_OPEN_API
WMKC_OF((wmkcVoid))
{
    static wmkcSize n, num[32];
    static wmkc_u32 x, count;

    for(x = 0; x < 32; ++x) {
        num[x] = rand() ^ (rand() ^ (rand() & rand()));
        n = n + (n ^ num[x]);
    }

    n = n + wmkcSwapByte(rand() & 0x0f);

    for(count = 0; count < 32; ++count) {
        for(x = 0; x < 32; ++x) {
            num[x] = rand() ^ (rand() ^ (rand() & rand()));
        }
        n = (n + (n ^ num[count])) ^ count;
        n = (n + rand()) ^ wmkcSwapByte((count + (rand() ^ (n - num[count]))) & 0xff);
    }

    return n;
}

WMKC_PUBLIC(wmkcSize) wmkcRandom_randint WMKC_OPEN_API
WMKC_OF((wmkcSize min, wmkcSize max))
{
    return wmkcRandom_rand() % (max - min + 1) + min;
}

WMKC_PUBLIC(wmkcVoid) wmkcRandom_urandom WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkc_u32 size))
{
#   if defined(WMKC_WINDOWS_SUPPORT)
    HCRYPTPROV hProv;
    CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0);
    CryptGenRandom(hProv, size, buf);
    CryptReleaseContext(hProv, 0);
#   elif defined(WMKC_LINUX_SUPPORT)
    getrandom(buf, size, GRND_RANDOM);
#   endif
}
