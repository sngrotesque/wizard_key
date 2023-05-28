#include <wmkc_time.h>

/**
 * @brief 获取当前的时间
 * @authors SN-Grotesque
 * 
 * 获取当前的时间，默认格式为：年月日-时分秒
 * 
 * @note 后续可以添加一个格式参数，由使用者自行决定时间格式。
 * @param dst 这是一个指针，指向结果指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcTime_GetNowTime WMKC_OPEN_API
WMKC_OF((wmkcChar **dst))
{
    wmkcErr_obj error;
    if(!dst) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcTime_GetNowTime: dst is NULL.");
    }
    wmkc_u32 dst_size = 24;
    if(!wmkcMemoryNew(wmkcChar *, (*dst), dst_size)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcTime_GetNowTime: (*dst) failed to apply for memory.");
    }
    time_t t;
    struct tm *tp;

    wmkcMemoryZero((*dst), dst_size);
    time(&t);
    tp = localtime(&t);

    strftime((*dst), dst_size, "%Y-%m-%d %H:%M:%S", tp);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 计时开始函数
 * @authors SN-Grotesque
 * 
 * 计时开始函数
 * 
 * @note 后续可以考虑使用Python的time.time()形式来实现不同批次的计时。
 * @param timer 这是一个指针，指向wmkcTime对象的地址。
 * @return 无
 */
WMKC_PUBLIC(wmkcVoid) wmkcTime_TimerBegin WMKC_OPEN_API
WMKC_OF((wmkcTime_obj *timer))
{
#if defined(WMKC_PLATFORM_LINUX)
    gettimeofday(&timer->start, wmkcNull);
#elif defined(WMKC_PLATFORM_WINOS) // #if defined(WMKC_PLATFORM_LINUX)
    timer->start = GetTickCount64();
#endif
}

/**
 * @brief 计时结束函数
 * @authors SN-Grotesque
 * 
 * 计时结束函数
 * 
 * @note 后续可以考虑使用Python的time.time()形式来实现不同批次的计时。
 * @param timer 这是一个指针，指向wmkcTime对象的地址。
 * @return 无
 */
WMKC_PUBLIC(wmkcVoid) wmkcTime_TimerEnd WMKC_OPEN_API
WMKC_OF((wmkcTime_obj *timer))
{
#if defined(WMKC_PLATFORM_LINUX)
    gettimeofday(&timer->stop, wmkcNull);
    timer->totalTime = (wmkcFloat)(CLOCKS_PER_SEC *
        (timer->stop.tv_sec - timer->start.tv_sec) +
        timer->stop.tv_usec - timer->start.tv_usec) /
        CLOCKS_PER_SEC;
#elif defined(WMKC_PLATFORM_WINOS)
    timer->totalTime = (wmkcFloat)(((wmkcFloat)GetTickCount64() -
        (wmkcFloat)timer->start) / CLOCKS_PER_SEC);
#endif
}

/**
 * @brief 用于打印计时结果的函数
 * @authors SN-Grotesque
 * 
 * 用于打印计时结果的函数
 * 
 * @note 无
 * @example wmkcTime_TimerPrint("timer: ", &timer);
 * @param text 这是一个指针，指向文本字符串的地址（要说的话）。
 * @param timer 这是一个指针，指向wmkcTime对象的地址。
 * @return 无
 */
WMKC_PUBLIC(wmkcVoid) wmkcTime_TimerPrint WMKC_OPEN_API
WMKC_OF((wmkcString text, wmkcTime_obj *timer))
{
    printf("%s%.2lfs.\n", text, timer->totalTime);
}
