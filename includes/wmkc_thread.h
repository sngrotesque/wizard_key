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
 * @ref https://man7.org/linux/man-pages/man3/sem_open.3.html
 * @ref https://man7.org/linux/man-pages/man3/sem_wait.3.html
 * @ref https://man7.org/linux/man-pages/man3/sem_post.3.html
 * @ref https://man7.org/linux/man-pages/man3/sem_destroy.3.html
 * @ref https://man7.org/linux/man-pages/man3/sem_close.3.html
 */
#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_THREAD
#define WMKC_THREAD

#include <semaphore.h>

#if defined(WMKC_PLATFORM_WINOS)
#include <Windows.h>
typedef HANDLE        wmkcThreadTh; // 线程句柄
typedef DWORD         wmkcThreadId; // 线程ID
typedef LPTHREAD_START_ROUTINE \
                    wmkcThreadFunc; // 函数指针
typedef DWORD WINAPI wmkcThreadRet; // 函数类型
#define WMKC_THREAD_DEFAULT_RETUEN 0
#elif defined(WMKC_PLATFORM_LINUX)
#include <pthread.h>
typedef pthread_t    wmkcThreadTh; // 线程句柄
typedef wmkc_s32     wmkcThreadId; // 线程ID
typedef wmkcVoid *(*wmkcThreadFunc)(wmkcVoid *); // 指针函数指针
typedef wmkcVoid   *wmkcThreadRet; // 函数类型
#define WMKC_THREAD_DEFAULT_RETUEN wmkcNull
#endif /* WMKC_PLATFORM_WINOS */

typedef wmkcVoid *wmkcThreadParam; // 函数参数

// wmkcThread句柄对象
typedef struct {
    wmkcThreadId id;
    wmkcThreadId flags;
    wmkcThreadTh th;
    sem_t sem;
} wmkcThread_handle_obj;

// wmkcThread对象
typedef struct {
    wmkcThread_handle_obj *handle; // 线程句柄
    wmkcThreadParam        args;   // 函数参数
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

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_Thread WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *thread, wmkcThreadFunc funcAddr));

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_join WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *thread));

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_start WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *thread));

#endif /* WMKC_THREAD */
#endif /* WMKC_SUPPORT */
