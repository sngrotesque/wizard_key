#include <wmkc_thread.h>

WMKC_PUBLIC(wmkcBool) _wmkcThread_create WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *obj, wmkcThreadFunc func, wmkcThreadParam param))
{
#   if defined(WMKC_PLATFORM_WINOS)
    // Windows API这个函数是失败时返回NULL
    if(!(obj->th = CreateThread(wmkcNull, 0, func, param, 0, &obj->id)))
        return false;
#   elif defined(WMKC_PLATFORM_LINUX)
    // 类Unix系统中，这个函数则是成功时返回0。
    if(pthread_create((*obj).th, 0, func, param))
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

WMKC_PUBLIC(wmkcErr_obj) wmkcThread_create WMKC_OPEN_API
WMKC_OF((wmkcThread_obj *obj, wmkcThreadFunc func, wmkcThreadParam param))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcThread_create: obj is NULL.");
    }
    if(!_wmkcThread_create(obj, func, param)) {
        wmkcErr_return(error, wmkcErr_ThreadCreate,
            "wmkcThread_create: Error creating thread.");
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PRIVATE(wmkcErr_obj) wmkcThread_join WMKC_OPEN_API
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


