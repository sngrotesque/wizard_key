#include <wmkc_object.h>

WMKC_PUBLIC(wmkcErr_obj) wmkcObj_new WMKC_OPEN_API
WMKC_OF((wmkc_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcObj_new", "obj is NULL.");
    }
    if(!wmkcMem_new(wmkc_obj*, (*obj), sizeof(wmkc_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcObj_new",
            "Failed to allocate memory for (*obj).");
    }

    (*obj)->buf = wmkcNull;
    (*obj)->size = 0;
    (*obj)->index = 0;
    (*obj)->found = false;

    wmkcErr_return(error, wmkcErr_OK, "wmkcObj_new", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcObj_free WMKC_OPEN_API
WMKC_OF((wmkc_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcObj_free", "obj is NULL.");
    }

    if((*obj)->buf) {
        wmkcMem_free((*obj)->buf);
    }
    wmkcMem_free((*obj));

    wmkcErr_return(error, wmkcErr_OK, "wmkcObj_free", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcObj_reverse WMKC_OPEN_API
WMKC_OF((wmkc_obj *obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcObj_reverse", "obj is NULL.");
    }
    wmkcFast wmkcByte swap;
    wmkcFast wmkcSize i;

    for(i = 0; i < (obj->size >> 1); ++i) {
        swap = obj->buf[i];
        obj->buf[i] = obj->buf[obj->size - i - 1];
        obj->buf[obj->size - i - 1] = swap;
    }

    wmkcErr_return(error, wmkcErr_OK, "wmkcObj_reverse", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcObj_append WMKC_OPEN_API
WMKC_OF((wmkc_obj *obj, wmkcCSTR content, wmkcSize size))
{
    wmkcErr_obj error;
    if(!obj || !content) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcObj_append",
            "obj or content is NULL.");
    }

    wmkcByte *p = wmkcNull;
    if(!size) size = strlen(content);

    if(obj->buf) {
        if(!wmkcMem_append(wmkcByte *, p, obj->buf, obj->size + size + 1)) {
            wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcObj_append",
                "Failed to allocate memory for p.");
        }
        memcpy(p + obj->size, content, size);
        obj->buf = p;
    } else {
        if(!wmkcMem_new(wmkcByte *, obj->buf, size + 1)) {
            wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcObj_append",
                "Failed to allocate memory for obj->buf.");
        }
        memcpy(obj->buf, content, size);
    }
    obj->size += size;
    obj->buf[obj->size] = 0x00;

    wmkcErr_return(error, wmkcErr_OK, "wmkcObj_append", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcObj_find WMKC_OPEN_API
WMKC_OF((wmkc_obj *obj, wmkcCSTR content, wmkcSize size, wmkcBool findNext))
{
    wmkcErr_obj error;
    if(!obj || !content) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcObj_find",
            "obj or content is NULL.");
    }
    wmkcSize i;
    wmkcBool findFlag = false;
    if(!size) size = strlen(content);

    for(i = (findNext)?((obj->found)?(obj->index+1):(obj->index)):(0); i < obj->size; ++i) {
        if(!memcmp(obj->buf + i, content, size)) {
            findFlag = true;
            if(!obj->found)
                obj->found = true;
            obj->index = i;
            break;
        }
    }

    if(!findFlag) {
        wmkcErr_return(error, wmkcErr_ErrOutRange, "wmkcObj_find", "No substring found.");
    }
    wmkcErr_return(error, wmkcErr_OK, "wmkcObj_find", "OK.");
}
