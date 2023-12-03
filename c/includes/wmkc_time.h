#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_TIME
#define WMKC_TIME
#include <wmkc_error.h>
#include <wmkc_memory.h>
#include <time.h>
#include <sys/timeb.h>

#ifdef WMKC_PLATFORM_WINOS
#include <Windows.h>
#elif defined(WMKC_PLATFORM_LINUX)
#include <sys/time.h>
#endif

typedef struct {
#if defined(WMKC_PLATFORM_LINUX)
    struct timeval start;
    struct timeval stop;
#elif defined(WMKC_PLATFORM_WINOS)
    ULONGLONG start;
    ULONGLONG stop;
#endif
    double totalTime;
} wmkcTime_obj;

/**
 * @brief 获取当前的时间
 * @authors SN-Grotesque
 * 
 * 获取当前的时间，默认格式为：年月日-时分秒
 * 
 * @note 后续可以添加一个格式参数，由使用者自行决定时间格式。
 * @param dst 指针，指向结果指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcTime_GetNowTime WMKC_OPEN_API
WMKC_OF((wmkcChar **dst));

/**
 * @brief 计时开始函数
 * @authors SN-Grotesque
 * 
 * 计时开始函数
 * 
 * @note 后续可以考虑使用Python的time.time()形式来实现不同批次的计时。
 * @param timer 指针，指向wmkcTime对象的地址。
 * @return 无
 */
WMKC_PUBLIC(wmkcVoid) wmkcTime_TimerBegin WMKC_OPEN_API
WMKC_OF((wmkcTime_obj *timer));

/**
 * @brief 计时结束函数
 * @authors SN-Grotesque
 * 
 * 计时结束函数
 * 
 * @note 后续可以考虑使用Python的time.time()形式来实现不同批次的计时。
 * @param timer 指针，指向wmkcTime对象的地址。
 * @return 无
 */
WMKC_PUBLIC(wmkcVoid) wmkcTime_TimerEnd WMKC_OPEN_API
WMKC_OF((wmkcTime_obj *timer));

/**
 * @brief 用于打印计时结果的函数
 * @authors SN-Grotesque
 * 
 * 用于打印计时结果的函数
 * 
 * @note 无
 * @example wmkcTime_TimerPrint("timer: ", &timer);
 * @param text 指针，指向文本字符串的地址（要说的话）。
 * @param timer 指针，指向wmkcTime对象的地址。
 * @return 无
 */
WMKC_PUBLIC(wmkcVoid) wmkcTime_TimerPrint WMKC_OPEN_API
WMKC_OF((wmkcCSTR text, wmkcTime_obj *timer));

WMKC_PUBLIC(wmkcVoid) wmkcTime_sleep WMKC_OPEN_API
WMKC_OF((double _time));

#endif /* WMKC_TIME */
#endif /* WMKC_SUPPORT */
