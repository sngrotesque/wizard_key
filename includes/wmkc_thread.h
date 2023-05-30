/**
 * @brief wmkc_thread库的参考链接
 * @ref https://learn.microsoft.com/zh-cn/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
 * @ref https://learn.microsoft.com/zh-cn/windows/win32/api/processthreadsapi/nf-processthreadsapi-resumethread
 * @ref https://man7.org/linux/man-pages/man3/pthread_create.3.html
 * @ref https://man7.org/linux/man-pages/man3/pthread_join.3.html
 * @ref https://man7.org/linux/man-pages/man3/pthread_kill.3.html
 * @ref https://man7.org/linux/man-pages/man3/pthread_exit.3.html
 * @ref https://man7.org/linux/man-pages/man3/pthread_self.3.html
 * @ref https://man7.org/linux/man-pages/man3/pthread_mutexattr_init.3.html
 * @ref https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html
 * @ref https://man7.org/linux/man-pages/man3/pthread_mutexattr_setrobust.3.html
 * @ref https://man7.org/linux/man-pages/man3/pthread_mutex_consistent.3.html
 * @ref https://man7.org/linux/man-pages/man3/pthread_cond_signal.3p.html
 * @ref https://man7.org/linux/man-pages/man3/sem_init.3.html
 * @ref https://man7.org/linux/man-pages/man3/sem_wait.3.html
 * @ref https://man7.org/linux/man-pages/man3/sem_post.3.html
 * @ref https://man7.org/linux/man-pages/man3/sem_destroy.3.html
 * @ref https://man7.org/linux/man-pages/man3/sem_close.3.html
 */
#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_THREAD
#define WMKC_THREAD

#if defined(WMKC_PLATFORM_WINOS)
#include <Windows.h>
typedef HANDLE        _wmkcThreadTh; // 线程句柄
typedef DWORD         _wmkcThreadId; // 线程ID
typedef LPTHREAD_START_ROUTINE _wmkcThreadFunc; // 函数指针
typedef LPVOID     _wmkcThreadParam; // 函数参数
typedef DWORD WINAPI _wmkcThreadRet; // 函数类型
#elif defined(WMKC_PLATFORM_LINUX)
#include <pthread.h>
typedef pthread_t *  _wmkcThreadTh; // 线程句柄
typedef wmkcSSize    _wmkcThreadId; // 线程ID
typedef wmkcVoid *(*_wmkcThreadFunc)(wmkcVoid *); // 指针函数指针
typedef wmkcVoid *_wmkcThreadParam; // 函数参数
typedef wmkcVoid   *_wmkcThreadRet; // 函数类型
#endif

typedef _wmkcThreadTh    wmkcThreadTh;    // 线程句柄
typedef _wmkcThreadId    wmkcThreadId;    // 线程ID
typedef _wmkcThreadFunc  wmkcThreadFunc;  // 函数指针或指针函数指针
typedef _wmkcThreadParam wmkcThreadParam; // 函数参数
typedef _wmkcThreadRet   wmkcThreadRet;   // 函数类型

// wmkcThread对象
typedef struct {
    wmkcThreadTh th; // 线程句柄
    wmkcThreadId id; // 线程ID
} wmkcThread_obj;

/**
 * @brief 为wmkcThread对象申请内存空间
 * @authors SN-Grotesque
 * @note 后续给wmkcThread对象添加一个成员，用于标识当前的线程数量。
 * @param obj 这是一个指针，指向wmkcThread对象指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcThread_new WMKC_OPEN_API
WMKC_OF((wmkcThread_obj **obj));

/**
 * @brief 为wmkcThread对象释放内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param obj 这是一个指针，指向wmkcThread对象指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcThread_free WMKC_OPEN_API
WMKC_OF((wmkcThread_obj **obj));

/**
 * @brief 创建线程并立即运行
 * @authors SN-Grotesque
 * @note 未完成
 * @param obj 这是一个指针，指向wmkcThread对象的地址。
 * @param func 这是一个指针，在Linux中为指针函数指针，在Win中为函数指针。
 * @param param 这是一个指针，代表func的参数，指向空类型的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcThread_create WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *obj, wmkcThreadFunc func, wmkcThreadParam param));

/**
 * @brief 等待线程执行完毕（阻塞线程）
 * @authors SN-Grotesque
 * @note 未完成
 * @param obj 这是一个指针，指向wmkcThread对象的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcThread_join WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *obj));

#endif /* WMKC_THREAD */
#endif /* WMKC_SUPPORT */
