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
typedef LPTHREAD_START_ROUTINE _wmkcThreadFunc; // 指针，指向函数
typedef HANDLE _wmkcThreadTh;                   // 线程句柄
#elif defined(WMKC_PLATFORM_LINUX)
#include <pthread.h>
typedef wmkcVoid *_wmkcThreadFunc;   // 指针，指向函数
typedef pthread_t _wmkcThreadTh;        // 线程句柄
#endif
typedef _wmkcThreadFunc wmkcThreadFunc; // 指针，指向函数
typedef _wmkcThreadTh wmkcThreadTh;     // 线程句柄





#endif /* WMKC_THREAD */
#endif /* WMKC_SUPPORT */
