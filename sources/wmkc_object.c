#include <wmkc_object.h>

WMKC_PUBLIC(wmkcErr_obj) wmkcObj_new WMKC_OPEN_API
WMKC_OF((wmkc_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcObj_new", "obj is NULL.");
    }
    if(!wmkcMem_new(wmkc_obj*, (*obj), sizeof(wmkc_obj))) {
        wmkcErr_func_return(error, wmkcErr_ErrMemory, "wmkcObj_new",
            "Failed to allocate memory for (*obj).");
    }

    (*obj)->buf = wmkcNull;
    (*obj)->size = 0;
    (*obj)->memSize = 0;

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcObj_new", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcObj_free WMKC_OPEN_API
WMKC_OF((wmkc_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcObj_free", "obj is NULL.");
    }

    if((*obj)->buf) {
        wmkcMem_free((*obj)->buf);
    }
    wmkcMem_free((*obj));

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcObj_free", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcObj_append WMKC_OPEN_API
WMKC_OF((wmkc_obj *obj, wmkcCSTR content))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcObj_free", "obj is NULL.");
    }

    wmkcByte *new_p = wmkcNull;
    wmkcSize content_size = strlen(content);

    if(!obj->buf) {
        if(!wmkcMem_new(wmkcByte *, obj->buf, content_size)) {
            wmkcErr_func_return(error, wmkcErr_ErrMemory, "wmkcObj_append",
                "Failed to allocate memory for obj->buf.");
        }
        memcpy(obj->buf, content, content_size);
    } else {
        if(!wmkcMem_append(wmkcByte *, new_p, obj->buf, obj->memSize + content_size)) {
            wmkcErr_func_return(error, wmkcErr_ErrMemory, "wmkcObj_append",
                "Failed to allocate memory for new_p.");
        }
        memcpy(new_p + obj->size, content, content_size);
        obj->buf = new_p;
    }
    obj->size += content_size;
    obj->memSize += content_size;

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcObj_append", "OK.");
}

