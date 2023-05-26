#include <wmkc_random.h>

/**
 * @brief 随机数种子
 * @authors SN-Grotesque
 * 
 * 此函数获取当前时间戳（精度为微秒）作为随机数种子传递到srand函数中。
 * 
 * @note 我认为这个函数后续可以重新设计一下，因为当前是只能使用时间
 *       作为种子。
 * @param 无
 * @return 无
*/
WMKC_PUBLIC(wmkcVoid) wmkcRandom_seed WMKC_OPEN_API
WMKC_OF((wmkcVoid))
{
    wmkcSize timeTotal;

#if defined(WMKC_PLATFORM_LINUX)
    struct timeval tv;
    mingw_gettimeofday(&tv, wmkcNull);
    timeTotal = (wmkcSize)tv.tv_sec * 1000000ULL + (wmkcSize)tv.tv_usec;
#elif defined(WMKC_PLATFORM_WINOS)
    FILETIME ft;
    LARGE_INTEGER li;
    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    timeTotal = (li.QuadPart - 116444736000000000ULL) / 10;
#endif

    srand((unsigned int)timeTotal);
}

/**
 * @brief 随机整数
 * @authors SN-Grotesque
 * 
 * 此函数通过一系列运算尽可能的将rand()函数提供的值混淆以提高部分安全性。
 * 
 * @note 无
 * @param 无
 * @return 返回一个wmkcSize变量，为随机整数。
*/
WMKC_PUBLIC(wmkcSize) wmkcRandom_rand WMKC_OPEN_API
WMKC_OF((wmkcVoid))
{
    wmkcSize n, num[32];
    wmkc_u32 x, count;

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

/**
 * @brief 返回一个范围内的随机整数数
 * @authors SN-Grotesque
 * 
 * 此函数通过wmkcRandom_rand函数获取一个min至max间的随机整数。
 * 
 * @note 无
 * @param min 代表此范围的最小值
 * @param max 代表此范围的最大值
 * @return 返回一个wmkcSize变量，为此指定范围内的随机整数。
*/
WMKC_PUBLIC(wmkcSize) wmkcRandom_randint WMKC_OPEN_API
WMKC_OF((wmkcSize min, wmkcSize max))
{
    return wmkcRandom_rand() % (max - min + 1) + min;
}

/**
 * @brief 返回一个由操作系统提供的安全随机数序列
 * @authors SN-Grotesque
 * 
 * 此函数通过调用系统对应的安全随机数函数获取一个随机序列。
 * 可用作加密算法密钥等需要安全性的随机序列。
 * 
 * @note 无
 * @param buf 这是一个指针，指向用于报错结果的序列的地址。
 * @param size 这是一个长度，代表buf指针指向的内容的长度
 *            （需要获取的随机序列的长度）。
 * @return 返回一个wmkcErr_ctx对象，用于告知使用者是否出错。
*/
WMKC_PUBLIC(wmkcErr_ctx) wmkcRandom_urandom WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkc_u32 size))
{
    wmkcErr_ctx error;
    if(!buf || !size) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "");
    }

#   if defined(WMKC_PLATFORM_WINOS)
    HCRYPTPROV hProv;
    CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0);
    CryptGenRandom(hProv, size, buf);
    CryptReleaseContext(hProv, 0);
#   elif defined(WMKC_PLATFORM_LINUX)
    getrandom(buf, size, GRND_RANDOM);
#   endif

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
