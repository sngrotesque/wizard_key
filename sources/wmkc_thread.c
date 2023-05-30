/**
 * @brief 一些说明
 * @note 此wmkc_thread库后续将实现为类似Python中的threading库的语法。
 * @note 比如使用Thread函数直接创建一个线程，但是挂起。等待用户使用run或者start函数来唤醒。
 */
#include <wmkc_thread.h>

WMKC_PRIVATE(wmkcBool) _wmkcThread_create
WMKC_OF((wmkcThread_obj *obj, wmkcThreadFunc func, wmkcThreadParam param))
{
#   if defined(WMKC_PLATFORM_WINOS)
    // Windows API这个函数是失败时返回NULL
    if(!(obj->th = CreateThread(wmkcNull, 0, func, param, CREATE_SUSPENDED, &obj->id)))
        return false;
#   elif defined(WMKC_PLATFORM_LINUX)
    // 类Unix系统中，这个函数则是成功时返回0。
    if(pthread_create(obj->th, wmkcNull, func, param))
        return false;
#   endif
    return true;
}

WMKC_PRIVATE(wmkcBool) _wmkcThread_join
WMKC_OF((wmkcThread_obj *obj))
{
#   if defined(WMKC_PLATFORM_WINOS)
    // Windows API这个函数是失败时返回-1
    if(WaitForSingleObject(obj->th, INFINITE) == wmkcErr_Err32)
        return false;
#   elif defined(WMKC_PLATFORM_LINUX)
    // 类Unix系统中，这个函数则是成功时返回0。
    if(pthread_join(*obj->th, NULL))
        return false;
#   endif
    return true;
}

WMKC_PRIVATE(wmkcBool) _wmkcThread_start
WMKC_OF((wmkcThread_obj *obj))
{
#   if defined(WMKC_PLATFORM_WINOS)
    if(ResumeThread(obj->th) == wmkcErr_Err32)
        return false;
#   elif defined(WMKC_PLATFORM_LINUX)

#endif
    return true;
}

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_new WMKC_OPEN_API
WMKC_OF((wmkcThread_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcThread_new: obj is NULL.");
    }
    if(!wmkcMemoryNew(wmkcThread_obj *, (*obj), sizeof(wmkcThread_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcThread_new: (*obj) failed to apply for memory.");
    }
#   if defined(WMKC_PLATFORM_LINUX)
    if(wmkcMemoryNew(wmkcThreadTh, (*obj)->th, sizeof(wmkcThreadTh))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcThread_new: (*obj)->th failed to apply for memory.");
    }
#   elif defined(WMKC_PLATFORM_WINOS)
    (*obj)->th = wmkcNull;
#   endif

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_free WMKC_OPEN_API
WMKC_OF((wmkcThread_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcThread_free: obj is NULL.");
    }
#   if defined(WMKC_PLATFORM_LINUX)
    wmkcMemoryFree((*obj)->th);
#   endif
    wmkcMemoryFree((*obj));

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_create WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *obj, wmkcThreadFunc func, wmkcThreadParam param))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcThread_create: obj is NULL.");
    }
#   ifdef WMKC_PLATFORM_LINUX
    if(!obj->th) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcThread_create: obj->th is NULL.");
    }
#   endif

    if(!_wmkcThread_create(obj, func, param)) {
        wmkcErr_return(error, wmkcErr_ThreadCreate,
            "wmkcThread_create: Error creating thread.");
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_start WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcThread_start: obj is NULL.");
    }
    if(!_wmkcThread_start(obj)) {
        wmkcErr_return(error, wmkcErr_ThreadStart,
            "wmkcThread_start: Thread startup error.");
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_join WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcThread_join: obj is NULL.");
    }
    if(!_wmkcThread_join(obj)) {
        wmkcErr_return(error, wmkcErr_ThreadJoin,
            "wmkcThread_join: Thread blocking error.");
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}


