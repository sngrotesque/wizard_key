#include <wmkc_object.h>

/**
 * @brief KMP算法实现
 * @authors SN-Grotesque
 * @param ms Main String主串
 * @param ss Sub String子串（模式串）
 * @param msSize 主串长度，为0时调用strlen获取长度
 * @param ssSize 子串长度，为0时调用strlen获取长度
 * @return wmkcSize，为-1时代表未找到指定子串。
 */
WMKC_PRIVATE(wmkcSize) wmkcObj_KMP
WMKC_OF((const wmkcByte *ms, const wmkcByte *ss, wmkcSize msSize, wmkcSize ssSize))
{
    wmkcSize i = 1, j = 0, record = -1;
    wmkcByte *next = wmkcNull;

    if(!msSize)
        msSize = strlen(ms);
    if(!ssSize)
        ssSize = strlen(ss);
    if(!wmkcMem_new(wmkcByte *, next, ssSize))
        return -1;

    while(i < ssSize) {
        (ss[j]==ss[i])?(next[i++] = ++j):((!j)?(next[i++]=0):(j=next[j-1]));
    }
    i = j = 0;
    while(i < msSize) {
        (ms[i]==ss[j])?(i++,j++):(j?(j=next[j-1]):(i++));
        if(j == ssSize) {
            record = i - j;
            break;
        }
    }
    wmkcMem_free(next);
    return record;
}

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
            if(!obj->found)
                obj->found = true;
            findFlag = true;
            obj->index = i;
            break;
        }
    }

    if(!findFlag) {
        wmkcErr_return(error, wmkcErr_ErrOutRange, "wmkcObj_find", "No substring found.");
    }
    wmkcErr_return(error, wmkcErr_OK, "wmkcObj_find", "OK.");
}

/**
 * @brief 此函数使用KMP算法时针对“查找下一个”项时会重复下标
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcObj_find_kmp WMKC_OPEN_API
WMKC_OF((wmkc_obj *obj, wmkcCSTR sub, wmkcSize subSize, wmkcBool findNext))
{
    wmkcErr_obj error;
    if(!obj || !sub) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcObj_find", "obj or sub is NULL.");
    }
    if(!subSize) subSize = strlen(sub);
    wmkcSize pos;

    if(!findNext) {
        obj->index = wmkcObj_KMP(obj->buf, sub, obj->size, subSize);
        if(obj->index == wmkcErr_Err64) {
            wmkcErr_return(error, wmkcErr_ErrOutRange, "wmkcObj_find", "No substring found.");
        }
    }

    wmkcErr_return(error, wmkcErr_OK, "wmkcObj_find", "OK.");
}
