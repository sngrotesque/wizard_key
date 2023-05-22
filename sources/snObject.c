#include <snObject.h>

SN_PUBLIC(snErr_ctx) snObject_new SN_OPEN_API
SN_FUNC_OF((snObject **obj, snSize _memory_size, snBool _Clear_memory))
{
    snErr_ctx error;
    if(!obj) {
        snErr_return(error, snErr_ErrNullData, "snObject_new: obj is NULL.");
    }

    if(!snMemoryNew(snObject *, (*obj), sizeof(snObject))) {
        snErr_return(error, snErr_ErrMemory,
            "snObject_new: (*obj) failed to apply for memory.");
    }

    if(_memory_size) {
        if(!snMemoryNew(snByte *, (*obj)->buf, _memory_size)) {
            snErr_return(error, snErr_ErrMemory,
                "snObject_new: (*obj)->buf failed to apply for memory.");
        }
        if(_Clear_memory)
            memset((*obj)->buf, 0x00, _memory_size);
    } else {
        (*obj)->buf = snNull;
        (*obj)->size = 0;
    }
    (*obj)->memSize = _memory_size;

    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snErr_ctx) snObject_free SN_OPEN_API
SN_FUNC_OF((snObject **obj))
{
    snErr_ctx error;
    if(!obj) {
        snErr_return(error, snErr_ErrNullData, "snObject_new: obj is NULL.");
    }

    if((*obj)->buf) {
        snMemoryFree((*obj)->buf);
    }
    snMemoryFree((*obj));

    snErr_return(error, snErr_OK, "OK.");
}
