#include <wmkc_object.h>

WMKC_PUBLIC(wmkcErr_ctx) wmkcObject_new WMKC_OPEN_API
WMKC_OF((wmkcObject **obj, wmkcSize _memory_size, wmkcBool _Clear_memory))
{
    wmkcErr_ctx error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcObject_new: obj is NULL.");
    }

    if(!wmkcMemoryNew(wmkcObject *, (*obj), sizeof(wmkcObject))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcObject_new: (*obj) failed to apply for memory.");
    }

    if(_memory_size) {
        if(!wmkcMemoryNew(wmkcByte *, (*obj)->buf, _memory_size)) {
            wmkcErr_return(error, wmkcErr_ErrMemory,
                "wmkcObject_new: (*obj)->buf failed to apply for memory.");
        }
        if(_Clear_memory)
            memset((*obj)->buf, 0x00, _memory_size);
    } else {
        (*obj)->buf = wmkcNull;
        (*obj)->size = 0;
    }
    (*obj)->memSize = _memory_size;

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_ctx) wmkcObject_free WMKC_OPEN_API
WMKC_OF((wmkcObject **obj))
{
    wmkcErr_ctx error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcObject_new: obj is NULL.");
    }

    if((*obj)->buf) {
        wmkcMemoryFree((*obj)->buf);
    }
    wmkcMemoryFree((*obj));

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
