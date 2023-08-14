#include <network/wmkc_ssl.h>

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_new WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcSSL_new", "obj is NULL.");
    }

    if(!wmkcMem_new(wmkcSSL_obj *, (*obj), sizeof(wmkcSSL_obj))) {
        wmkcErr_func_return(error, wmkcErr_ErrMemory, "wmkcSSL_new",
            "Failed to allocate memory for (*obj).");
    }

    (*obj)->ctx = wmkcNull;
    (*obj)->ssl = wmkcNull;
    (*obj)->net = wmkcNull;

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcSSL_new", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_free WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcSSL_free", "obj is NULL.");
    }

    if((*obj)->ctx) {
        SSL_CTX_free((*obj)->ctx);
    }
    if((*obj)->ssl) {
        SSL_free((*obj)->ssl);
    }
    if((*obj)->net) {
        if((error = wmkcNet_free(&(*obj)->net)).code) {
            return error;
        }
    }
    wmkcMem_free((*obj));

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcSSL_free", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_Context WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj, const SSL_METHOD *_method))
{
    wmkcErr_obj error;
    if(!obj || !_method) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcSSL_Context",
            "obj or _method is NULL.");
    }

    obj->ctx = SSL_CTX_new(_method);
    obj->ssl = SSL_new(obj->ctx);

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcSSL_new", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_wrap_socket WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj))
{

}



