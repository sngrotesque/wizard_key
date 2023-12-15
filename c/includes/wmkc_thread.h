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
 * 
 * @author SN-Grotesque
 * @note 对于这个库，我目前不知道该如何添加功能了，因为平时对于多线程库的使用也非常少。
 *       如果你知道该如何优化这个库，请直接提交拉取请求和合并请求来告诉我。
 * 
 * 如果您想自行使用 C 语言开发一个同时支持 Linux 和 Win64 系统的多线程库，
 * 您需要了解这两个平台上的线程 API。在 Linux 系统上，您可以使用 POSIX 线程（pthread）库来实现多线程。
 * 在 Win64 系统上，您可以使用 Windows API 中的线程相关函数来实现多线程。
 * 您需要编写一些跨平台的代码来抽象出这两个平台上的线程 API 的差异，
 * 以便您的多线程库能够在这两个平台上运行。您可以参考一些现有的跨平台多线程库，
 * 例如 Boost 的 thread 库，来了解如何实现这一目标。
 * 此外，您还需要考虑一些其他问题，例如线程同步、互斥量、条件变量等。
 * 这些问题也需要您编写跨平台的代码来解决。
 * 
 * 线程同步、互斥量和条件变量都是多线程编程中常用的概念。
 * 线程同步：当多个线程需要访问共享资源时，为了避免冲突，需要对这些线程进行同步。
 * 线程同步通常通过使用锁来实现。锁可以保证在同一时间只有一个线程能够访问共享资源。
 * 互斥量：互斥量（mutex）是一种用于实现线程同步的锁。它可以保证在同一时间只有一个线程能够访问共享资源。
 * 当一个线程需要访问共享资源时，它会尝试获取互斥量。
 * 如果互斥量已经被其他线程占用，那么该线程会被阻塞，直到互斥量被释放。
 * 条件变量：条件变量（condition variable）是一种用于在线程之间进行通信的机制。
 * 它允许一个线程等待另一个线程发出的信号。当一个线程需要等待某个条件成立时，它可以使用条件变量来阻塞自己。
 * 当另一个线程改变了条件并发出信号时，被阻塞的线程会被唤醒。
 * 这些概念在多线程编程中非常重要，它们可以帮助您更好地管理多个线程之间的交互。
 */
#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_THREAD
#define WMKC_THREAD
#include <wmkc_error.h>
#include <wmkc_memory.h>
#include <semaphore.h>

#if defined(WMKC_PLATFORM_WINOS)
#include <Windows.h>
typedef HANDLE        wmkcThreadTh; // 线程句柄
typedef DWORD         wmkcThreadId; // 线程ID
typedef LPTHREAD_START_ROUTINE \
                    wmkcThreadFunc; // 函数指针
// typedef DWORD        wmkcThreadRet; // 函数类型
#define wmkcThreadRet DWORD WINAPI // 函数类型
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
 * @param obj 指针，指向wmkcThread对象指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcThread_new WMKC_OPEN_API
WMKC_OF((wmkcThread_obj **obj));

/**
 * @brief 为wmkcThread对象释放内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcThread对象指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcThread_free WMKC_OPEN_API
WMKC_OF((wmkcThread_obj **obj));

/**
 * @brief 使用wmkcThread对象创建一个线程
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcThread对象的地址。
 * @param funcAddr 指针，指向函数或函数指针。
 * @param funcArgs 这是线程参数，会将wmkcThread对象中的args成员指向此参数。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcThread_Thread WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *obj, wmkcThreadFunc funcAddr, wmkcThreadParam funcArgs));

/**
 * @brief 等待wmkcThread对象绑定的线程执行完毕
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcThread对象的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcThread_join WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *obj));

/**
 * @brief 调用此函数会将被挂起的线程唤醒。
 * @authors SN-Grotesque
 * @note 需要使用信号量（semaphore）在函数外对信号进行初始化，
 *       并在线程函数中添加阻塞线程的信号语句。
 *       至于为什么不使用WinAPI，是因为麻烦，后续可以优化的话再说吧。
 * @param obj 指针，指向wmkcThread对象的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcThread_start WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *obj));

#endif /* WMKC_THREAD */
#endif /* WMKC_SUPPORT */
