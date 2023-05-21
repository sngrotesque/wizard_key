#include <image/snPng.h>

SN_PUBLIC(snErr_ctx *) snPng_new SN_OPEN_API
SN_FUNC_OF((snPng_block **obj))
{
    snErr_ctx *error = snNull;
    snErr_new(error);
    
    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snErr_ctx *) snPng_free SN_OPEN_API
SN_FUNC_OF((snPng_block **obj))
{
    snErr_ctx *error = snNull;
    snErr_new(error);
    
    snErr_return(error, snErr_OK, "OK.");
}

