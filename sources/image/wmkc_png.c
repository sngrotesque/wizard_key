#include <image/wmkc_png.h>

WMKC_PUBLIC(wmkcErr_ctx) wmkcPng_new WMKC_OPEN_API
WMKC_OF((wmkcPng_block **obj))
{
    wmkcErr_ctx error;
    
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_ctx) wmkcPng_free WMKC_OPEN_API
WMKC_OF((wmkcPng_block **obj))
{
    wmkcErr_ctx error;
    
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

