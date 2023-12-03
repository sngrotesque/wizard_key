#include <wmkc_thread.h>

WMKC_PRIVATE(wmkcBool) _wmkcThread_create
WMKC_OF((wmkcThread_obj *obj, wmkcThreadFunc funcAddr))
{
#   if defined(WMKC_PLATFORM_LINUX)
    /**
     * 成功后，pthread_create函数返回0，
     * 出现错误时，返回一个错误号，并且*thread的内容是未定义的。
    */
    if(pthread_create(&obj->handle->th, wmkcNull, funcAddr, obj)) {
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
    obj->handle->th = CreateThread(wmkcNull, 0, funcAddr, obj, 0, wmkcNull);
    if(!obj) {
        return false;
    }
#   endif

    return true;
}

WMKC_PRIVATE(wmkcBool) _wmkcThread_join
WMKC_OF((wmkcThread_obj *obj))
{
#   if defined(WMKC_PLATFORM_WINOS)
    /**
     * 如果函数成功，则返回值指示导致函数返回的事件。 可以是下列值之一。
     * WAIT_ABANDONED  指定的对象是一个互斥对象，该对象不是由拥有互斥对象的线程在
     * 0x00000080L     拥有线程终止之前释放的。 互斥对象所有权授予调用线程，互斥状态
     *                 设置为非对齐状态。如果互斥体保护持久状态信息，则应检查该信息
     *                 是否一致性。
     * WAIT_OBJECT_0   指定对象的状态已发出信号。
     * 0x00000000L
     * 
     * WAIT_TIMEOUT    超时间隔已过，对象的状态未对齐。
     * 0x00000102L
     * 
     * WAIT_FAILED     函数失败。 要获得更多的错误信息，请调用 GetLastError。
     * (DWORD) 0xFFFFFFFF
     */
    if(WaitForSingleObject(obj->handle->th, INFINITE) == wmkcErr_Err32)
        return false;
#   elif defined(WMKC_PLATFORM_LINUX)
    /**
     * 成功后，pthread_join函数返回0，
     * 出现错误时，返回一个错误号。
    */
    if(pthread_join(obj->handle->th, NULL))
        return false;
#   endif
    return true;
}

WMKC_PRIVATE(wmkcBool) _wmkcThread_start
WMKC_OF((wmkcThread_obj *obj))
{
    if(sem_post(&obj->handle->sem)) {
        return false;
    }
    return true;
}

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_new WMKC_OPEN_API
WMKC_OF((wmkcThread_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcThread_new", "obj is NULL.");
    }
    if(!wmkcMem_new(wmkcThread_obj *, (*obj), sizeof(wmkcThread_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcThread_new",
            "Failed to allocate memory for (*obj).");
    }
    if(!wmkcMem_new(wmkcThread_handle_obj *, (*obj)->handle, sizeof(wmkcThread_handle_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcThread_new",
            "Failed to allocate memory for (*obj)->handle.");
    }

    wmkcErr_return(error, wmkcErr_OK, "wmkcThread_new", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_free WMKC_OPEN_API
WMKC_OF((wmkcThread_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcThread_free", "obj is NULL.");
    }

    if((*obj)->handle) {
        wmkcMem_free((*obj)->handle);
    }
    (*obj)->args = wmkcNull;

    wmkcErr_return(error, wmkcErr_OK, "wmkcThread_free", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_Thread WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *obj, wmkcThreadFunc funcAddr, wmkcThreadParam funcArgs))
{
    wmkcErr_obj error;
    if(!obj || !obj->handle) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcThread_Thread",
            "obj or obj->handle is NULL.");
    }
    obj->args = funcArgs;
    if(!_wmkcThread_create(obj, funcAddr)) {
        wmkcErr_return(error, wmkcErr_ThreadCreate, "wmkcThread_Thread",
            "Error creating obj.");
    }
    wmkcErr_return(error, wmkcErr_OK, "wmkcThread_Thread", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_join WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *obj))
{
    wmkcErr_obj error;
    if(!obj || !obj->handle) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcThread_join",
            "obj or obj->handle is NULL.");
    }
    if(!_wmkcThread_join(obj)) {
        wmkcErr_return(error, wmkcErr_ThreadJoin, "wmkcThread_join",
            "Thread blocking error.");
    }
    wmkcErr_return(error, wmkcErr_OK, "wmkcThread_join", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_start WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *obj))
{
    wmkcErr_obj error;
    if(!obj || !obj->handle) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcThread_start",
            "obj or obj->handle is NULL.");
    }
    if(!_wmkcThread_start(obj)) {
        wmkcErr_return(error, wmkcErr_ThreadStart, "wmkcThread_start",
            "Thread startup error.");
    }
    wmkcErr_return(error, wmkcErr_OK, "wmkcThread_start", "OK.");
}

