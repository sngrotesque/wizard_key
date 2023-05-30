/*
* https://learn.microsoft.com/zh-cn/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
*/

#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_THREAD
#define WMKC_THREAD

#include <pthread.h>

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
    wmkcSize  count; // 线程数量
} wmkcThread_obj;

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_create WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *obj, wmkcThreadFunc func, wmkcThreadParam param));

WMKC_PRIVATE(wmkcErr_obj) wmkcThread_join WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *obj));

#endif /* WMKC_THREAD */
#endif /* WMKC_SUPPORT */
