#include <network/snTransfer.h>

/**
 * 由于snNet库的不确定性，暂时弃用此传输库
*/

SN_PUBLIC(snError) snTransfer_new SN_OPEN_API
SN_FUNC_OF((snTransfer_ctx **ctx, snString addr, sn_u16 port, sn_u32 maxRetry))
{
    if(!addr) {
        return snErr_ErrNullData;
    }
    if(!snMemoryNew(snTransfer_ctx *, (*ctx), sizeof(snTransfer_ctx)))
        return snErr_Memory;

    (*ctx)->addr = addr;
    if(port) {
        (*ctx)->port = port;
    } else {
        (*ctx)->port = SN_FT_DEFAULT_PORT;
    }
    if(maxRetry) {
        (*ctx)->maxRetry = maxRetry;
    } else {
        (*ctx)->maxRetry = SN_FT_DEFAULT_MAX_RETRY;
    }
    (*ctx)->snc = snNull;
    // SNC_new(&(*ctx)->snc, SNC_256);
    return snErr_OK;
}

SN_PUBLIC(snError) snTransfer_free SN_OPEN_API
SN_FUNC_OF((snTransfer_ctx **ctx))
{
    if((*ctx)->snc) {
        SNC_release(&(*ctx)->snc);
    }
    snMemoryFree((*ctx));
    return snErr_OK;
}

SN_PUBLIC(snError) snTransfer_Listen SN_OPEN_API
SN_FUNC_OF((snTransfer_ctx *ctx, snString fn))
{
    if(!ctx->addr || !ctx->port) {
        return snErr_ErrNullData;
    }
    if(!ctx->maxRetry) {
        ctx->maxRetry = SN_FT_DEFAULT_MAX_RETRY;
    }

    return snErr_OK;
}

SN_PUBLIC(snError) snTransfer_Client SN_OPEN_API
SN_FUNC_OF((snTransfer_ctx *ctx, snString fn))
{
    if(!snFile_exists(fn))
        return snErr_FileFolderPath;
    if(!ctx->addr || !ctx->port) {
        return snErr_ErrNullData;
    }
    if(!ctx->maxRetry) {
        ctx->maxRetry = SN_FT_DEFAULT_MAX_RETRY;
    }

    return snErr_OK;
}