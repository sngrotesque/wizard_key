#include <snRand.h>

SN_PUBLIC(snVoid) snRand_Seed SN_OPEN_API
SN_FUNC_OF((snVoid))
{
#   if defined(__linux)
    clockid_t a = 0;
    struct timespec p;
    clock_gettime(a, &p);
    srand((unsigned)(p.tv_nsec));
#   elif defined(_WIN32)
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

SN_PUBLIC(snSize) snRand_Rand SN_OPEN_API
SN_FUNC_OF((snVoid))
{
    static snSize n, num[32];
    static sn_u32 x, count;

    for(x = 0; x < 32; ++x) {
        num[x] = rand() ^ (rand() ^ (rand() & rand()));
        n = n + (n ^ num[x]);
    }

    n = n + snByteSwap(rand() & 0x0f);

    for(count = 0; count < 32; ++count) {
        for(x = 0; x < 32; ++x) {
            num[x] = rand() ^ (rand() ^ (rand() & rand()));
        }
        n = (n + (n ^ num[count])) ^ count;
        n = (n + rand()) ^ snByteSwap((count + (rand() ^ (n - num[x]))) & 0xff);
    }

    return n;
}

SN_PUBLIC(snSize) snRand_Randint SN_OPEN_API
SN_FUNC_OF((snSize min, snSize max))
{
    return snRand_Rand() % (max - min + 1) + min;
}

SN_PUBLIC(snVoid) snRand_Urandom SN_OPEN_API
SN_FUNC_OF((snByte *buf, sn_u32 size))
{
#   if defined(_WIN32)
    HCRYPTPROV hProv;
    CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0);
    CryptGenRandom(hProv, size, buf);
    CryptReleaseContext(hProv, 0);
#   elif defined(__linux)
    getrandom(buf, size, GRND_RANDOM);
#   endif
}
