#include <snObject.h>

WMKC_PUBLIC(snErr_ctx) snObject_new WMKC_OPEN_API
WMKC_OF((snObject **obj, wmkcSize _memory_size, wmkcBool _Clear_memory))
{
    snErr_ctx error;
    if(!obj) {
        snErr_return(error, snErr_ErrNULL, "snObject_new: obj is NULL.");
    }

    if(!wmkcMemoryNew(snObject *, (*obj), sizeof(snObject))) {
        snErr_return(error, snErr_ErrMemory,
            "snObject_new: (*obj) failed to apply for memory.");
    }

    if(_memory_size) {
        if(!wmkcMemoryNew(wmkcByte *, (*obj)->buf, _memory_size)) {
            snErr_return(error, snErr_ErrMemory,
                "snObject_new: (*obj)->buf failed to apply for memory.");
        }
        if(_Clear_memory)
            memset((*obj)->buf, 0x00, _memory_size);
    } else {
        (*obj)->buf = wmkcNull;
        (*obj)->size = 0;
    }
    (*obj)->memSize = _memory_size;

    snErr_return(error, snErr_OK, "OK.");
}

WMKC_PUBLIC(snErr_ctx) snObject_free WMKC_OPEN_API
WMKC_OF((snObject **obj))
{
    snErr_ctx error;
    if(!obj) {
        snErr_return(error, snErr_ErrNULL, "snObject_new: obj is NULL.");
    }

    if((*obj)->buf) {
        wmkcMemoryFree((*obj)->buf);
    }
    wmkcMemoryFree((*obj));

    snErr_return(error, snErr_OK, "OK.");
}
