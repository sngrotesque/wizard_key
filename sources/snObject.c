#include <snObject.h>

SN_PUBLIC(snError) snObject_new SN_OPEN_API
SN_FUNC_OF((snObject **obj, snSize _memory_size, snBool _Clear_memory))
{
    if(!snMemoryNew(snObject *, (*obj), sizeof(snObject)))
        return snErr_Memory;

    if(_memory_size) {
        if(!snMemoryNew(snByte *, (*obj)->buf, _memory_size))
            return snErr_Memory;
        if(_Clear_memory)
            memset((*obj)->buf, 0x00, _memory_size);
    } else {
        (*obj)->buf = snNull;
        (*obj)->size = 0;
    }
    (*obj)->memSize = _memory_size;

    return snErr_OK;
}

SN_PUBLIC(snError) snObject_free SN_OPEN_API
SN_FUNC_OF((snObject **obj))
{
    if(!obj)
        return snErr_ErrNullData;

    snMemoryFree((*obj)->buf);
    snMemoryFree((*obj));

    return snErr_OK;
}
