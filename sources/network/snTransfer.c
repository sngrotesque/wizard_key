#include <network/snTransfer.h>

WMKC_PUBLIC(snErr_ctx) snTransfer_new WMKC_OPEN_API
WMKC_OF((snTransfer_ctx **net, wmkcString addr, wmkc_u16 port, wmkc_u32 maxRetry))
{
    snErr_ctx error;

    if(!net || !addr) {
        snErr_return(error, snErr_ErrNULL, "snTransfer_new: net or addr is NULL.");
    }
    if(!wmkcMemoryNew(snTransfer_ctx *, (*net), sizeof(snTransfer_ctx))) {
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
    (*net)->snc = wmkcNull;
    // SNC_new(&(*net)->snc, SNC_256);

    snErr_return(error, snErr_OK, "OK.");
}

WMKC_PUBLIC(snErr_ctx) snTransfer_free WMKC_OPEN_API
WMKC_OF((snTransfer_ctx **net))
{
    snErr_ctx error;
    if(!net) {
        snErr_return(error, snErr_ErrNULL, "snTransfer_free: net is NULL.");
    }

    if((*net)->snc) {
        SNC_release(&(*net)->snc);
    }
    wmkcMemoryFree((*net));

    snErr_return(error, snErr_OK, "OK.");
}

WMKC_PUBLIC(snErr_ctx) snTransfer_Listen WMKC_OPEN_API
WMKC_OF((snTransfer_ctx *net, snFileString fn))
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

WMKC_PUBLIC(snErr_ctx) snTransfer_Client WMKC_OPEN_API
WMKC_OF((snTransfer_ctx *net, snFileString fn))
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
