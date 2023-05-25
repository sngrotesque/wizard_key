#include <image/snPng.h>

WMKC_PUBLIC(snErr_ctx) snPng_new WMKC_OPEN_API
WMKC_OF((snPng_block **obj))
{
    snErr_ctx error;
    
    snErr_return(error, snErr_OK, "OK.");
}

WMKC_PUBLIC(snErr_ctx) snPng_free WMKC_OPEN_API
WMKC_OF((snPng_block **obj))
{
    snErr_ctx error;
    
    snErr_return(error, snErr_OK, "OK.");
}

