#include <network/snTransfer.h>

SN_PUBLIC(snErr_ctx) snTransfer_new SN_OPEN_API
SN_FUNC_OF((snTransfer_ctx **net, snString addr, sn_u16 port, sn_u32 maxRetry))
{
    snErr_ctx error;

    if(!net || !addr) {
        snErr_return(error, snErr_ErrNULL, "snTransfer_new: net or addr is NULL.");
    }
    if(!snMemoryNew(snTransfer_ctx *, (*net), sizeof(snTransfer_ctx))) {
        snErr_return(error, snErr_ErrMemory,
            "snTransfer_new: (*net) Failed to apply for memory.");
    }

    (*net)->addr = addr;
    if(port) {
        (*net)->port = port;
    } else {
        (*net)->port = SN_FT_DEFAULT_PORT;
    }
    if(maxRetry) {
        (*net)->maxRetry = maxRetry;
    } else {
        (*net)->maxRetry = SN_FT_DEFAULT_MAX_RETRY;
    }
    (*net)->snc = snNull;
    // SNC_new(&(*net)->snc, SNC_256);

    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snErr_ctx) snTransfer_free SN_OPEN_API
SN_FUNC_OF((snTransfer_ctx **net))
{
    snErr_ctx error;
    if(!net) {
        snErr_return(error, snErr_ErrNULL, "snTransfer_free: net is NULL.");
    }

    if((*net)->snc) {
        SNC_release(&(*net)->snc);
    }
    snMemoryFree((*net));

    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snErr_ctx) snTransfer_Listen SN_OPEN_API
SN_FUNC_OF((snTransfer_ctx *net, snFileStr fn))
{
    snErr_ctx error;
    if(!net || !net->addr || !net->port || !fn) {
        snErr_return(error, snErr_ErrNULL,
            "snTransfer_Listen: net or net->addr or net->port or fn is NULL.");
    }
    if(!net->maxRetry) {
        net->maxRetry = SN_FT_DEFAULT_MAX_RETRY;
    }

    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snErr_ctx) snTransfer_Client SN_OPEN_API
SN_FUNC_OF((snTransfer_ctx *net, snFileStr fn))
{
    snErr_ctx error;
    if(!net || !net->addr || !net->port || !fn) {
        snErr_return(error, snErr_ErrNULL,
            "snTransfer_Listen: net or net->addr or net->port or fn is NULL.");
    }
    if(!net->maxRetry) {
        net->maxRetry = SN_FT_DEFAULT_MAX_RETRY;
    }

    snErr_return(error, snErr_OK, "OK.");
}
