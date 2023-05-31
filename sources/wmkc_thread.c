#include <wmkc_thread.h>

WMKC_PRIVATE(wmkcBool) _wmkcThread_create
WMKC_OF((wmkcThread_obj *thread, wmkcThreadFunc funcAddr))
{
#   if defined(WMKC_PLATFORM_LINUX)
    /**
     * 成功后，pthread_create函数返回0，
     * 出现错误时，返回一个错误号，并且*thread的内容是未定义的。
    */
    if(pthread_create(&thread->handle->th, wmkcNull, funcAddr, thread)) {
        return false;
    }
#   elif defined(WMKC_PLATFORM_WINOS)
    /**
     * 如果函数成功，则返回值是新线程的句柄。
     * 如果函数失败，则返回值为 NULL。 要获得更多的错误信息，请调用 GetLastError。
     * 请注意，即使 lpStartAddress 指向数据、代码或无法访问，CreateThread 也可能成功。
     * 如果线程运行时启动地址无效，则会发生异常，线程将终止。
     * 由于启动地址无效，线程终止作为线程进程的错误退出进行处理。
     * 此行为类似于 CreateProcess 的异步性质，即使它引用无效或缺少动态链接库 () ，也会创建进程。
    */
    thread->handle->th = CreateThread(wmkcNull, 0, funcAddr, thread, 0, wmkcNull);
    if(!thread) {
        return false;
    }
#   endif

    return true;
}

WMKC_PRIVATE(wmkcBool) _wmkcThread_join
WMKC_OF((wmkcThread_obj *thread))
{
#   if defined(WMKC_PLATFORM_WINOS)
    // Windows API这个函数是失败时返回-1
    if(WaitForSingleObject(thread->handle->th, INFINITE) == wmkcErr_Err32)
        return false;
#   elif defined(WMKC_PLATFORM_LINUX)
    // 类Unix系统中，这个函数则是成功时返回0。
    if(pthread_join(thread->handle->th, NULL))
        return false;
#   endif
    return true;
}

WMKC_PRIVATE(wmkcBool) _wmkcThread_start
WMKC_OF((wmkcThread_obj *thread))
{
    if(sem_post(&thread->handle->sem)) {
        return false;
    }
    return true;
}

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_new WMKC_OPEN_API
WMKC_OF((wmkcThread_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcThread_new: obj is NULL.");
    }
    printf("wmkcThread_new test: 00\n");
    if(!wmkcMemoryNew(wmkcThread_obj *, (*obj), sizeof(wmkcThread_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcThread_new: (*obj) failed to apply for memory.");
    }
    printf("wmkcThread_new test: 01\n");
    if(!wmkcMemoryNew(wmkcThread_handle_obj *, (*obj)->handle, sizeof(wmkcThread_handle_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcThread_new: (*obj)->handle failed to apply for memory.");
    }
    printf("wmkcThread_new test: 02\n");

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_free WMKC_OPEN_API
WMKC_OF((wmkcThread_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcThread_new: obj is NULL.");
    }

    if((*obj)->handle) {
        wmkcMemoryFree((*obj)->handle);
    }
    (*obj)->args = wmkcNull;

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_Thread WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *thread, wmkcThreadFunc funcAddr))
{
    wmkcErr_obj error;
    if(!thread || !thread->handle) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcThread_Thread: thread or thread->handle is NULL.");
    }
    if(!_wmkcThread_create(thread, funcAddr)) {
        wmkcErr_return(error, wmkcErr_ThreadCreate,
            "wmkcThread_Thread: Error creating thread.");
    }
}

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_join WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *thread))
{
    wmkcErr_obj error;
    if(!thread || !thread->handle) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcThread_join: thread or thread->handle is NULL.");
    }
    if(!_wmkcThread_join(thread)) {
        wmkcErr_return(error, wmkcErr_ThreadJoin,
            "wmkcThread_join: Thread blocking error.");
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_start WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *thread))
{
    wmkcErr_obj error;
    if(!thread || !thread->handle) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcThread_start: thread or thread->handle is NULL.");
    }
    if(!_wmkcThread_start(thread)) {
        wmkcErr_return(error, wmkcErr_ThreadStart,
            "wmkcThread_start: Thread startup error.");
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

