#include <wmkc_thread.h>

WMKC_PRIVATE(wmkcBool) _wmkcThread_create
WMKC_OF((wmkcThreadTh th, wmkcThreadFunc *func, wmkcVoid *param))
{
#   if defined(WMKC_PLATFORM_WINOS)
    DWORD thId;
    th = CreateThread(NULL, 0, func, param, 0, &thId);
#   elif defined(WMKC_PLATFORM_LINUX)

#   endif
}

WMKC_PRIVATE(wmkcBool) _wmkcThread_join
WMKC_OF((HANDLE th))
{
    WaitForSingleObject(th, INFINITE);
}


WMKC_PUBLIC(wmkcErr_obj) wmkcThread_create WMKC_OPEN_API
WMKC_OF((wmkcThreadFunc *func, wmkcVoid *param))
{
    wmkcErr_obj error;

    






    wmkcErr_return(error, wmkcErr_OK, "OK.");
}


