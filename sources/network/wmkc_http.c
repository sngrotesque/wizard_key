#include <network/wmkc_http.h>

WMKC_PUBLIC(wmkcErr_obj) wmkcHttp_parseURL WMKC_OPEN_API
WMKC_OF((wmkcHttp_obj *dst, wmkcCSTR url))
{
    wmkcErr_obj error;
    if(!dst || !url) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "dst or url is NULL.");
    }









    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

