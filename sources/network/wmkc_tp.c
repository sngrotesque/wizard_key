#include <network/wmkc_tp.h>

WMKC_PRIVATE(wmkcErr_obj) wmkcTp_crypto_new
WMKC_OF((wmkcTp_obj *obj, SNC_mode mode, wmkcByte *key, wmkcByte *iv))
{
    wmkcErr_obj error;
    if(!obj || !key || !iv) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcTp_crypto_init: "
            "obj or key or iv is NULL.");
    }

    error = wmkcSNC_new(&obj->snc, mode);
    if(error.code) return error;
    error = wmkcSNC_init(obj->snc, key, iv);
    if(error.code) return error;

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PRIVATE(wmkcErr_obj) wmkcTp_net_new
WMKC_OF((wmkcTp_obj *obj, wmkcNetType family))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcTp_net_new: obj is NULL.");
    }
    wmkcNet_init(wmkcNull, wmkcNull, 0);

    error = wmkcNet_new(&obj->net, wmkcNull, family, false);
    if(error.code) return error;
    error = wmkcNet_init(obj->net, obj->addr, obj->port);
    if(error.code) return error;

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcTp_new WMKC_OPEN_API
WMKC_OF((wmkcTp_obj **obj, wmkcCSTR addr, wmkc_u16 port, wmkc_u32 maxRetry,
    wmkcNetType family, SNC_mode mode, wmkcByte *key, wmkcByte *iv))
{
    wmkcErr_obj error;
    if(!obj || !addr) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcTp_new: obj or addr is NULL.");
    }
    if(!wmkcMem_new(wmkcTp_obj *, (*obj), sizeof(wmkcTp_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcTp_new: Failed to allocate memory for (*obj).");
    }
    (*obj)->snc = wmkcNull;
    (*obj)->net = wmkcNull;

    (*obj)->addr = addr;
    if(!port) {
        (*obj)->port = WMKC_TP_DEFAULT_PORT;
    } else {
        (*obj)->port = port;
    }
    if(!maxRetry) {
        (*obj)->maxRetry = WMKC_TP_DEFAULT_MAX_RETRY;
    } else {
        (*obj)->maxRetry = maxRetry;
    }

    error = wmkcTp_crypto_new((*obj), mode, key, iv);
    if(error.code) {
        wmkcMem_free((*obj));
        return error;
    }
    error = wmkcTp_net_new((*obj), family);
    if(error.code) {
        wmkcMem_free((*obj));
        return error;
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcTp_free WMKC_OPEN_API
WMKC_OF((wmkcTp_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcTp_free: obj is NULL.");
    }
    if((*obj)->net) {
        wmkcNet_close((*obj)->net);
        error = wmkcNet_free(&(*obj)->net);
        if(error.code) return error;
    }
    if((*obj)->snc) {
        error = wmkcSNC_free(&(*obj)->snc);
        if(error.code) return error;
    }

    wmkcMem_free((*obj));

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcTp_sender WMKC_OPEN_API
WMKC_OF((wmkcCSTR addr, wmkc_u16 port, wmkc_u32 maxRetry))
{
    wmkcTp_obj *tp = wmkcNull;
    wmkcNet_obj *net = wmkcNull;
}

WMKC_PUBLIC(wmkcErr_obj) wmkcTp_receiver WMKC_OPEN_API
WMKC_OF((wmkcCSTR addr, wmkc_u16 port, wmkc_u32 maxRetry))
{

}
