#include <wmkc_stream.h>

WMKC_PUBLIC(wmkcErr_obj) wmkcStream_Splicing WMKC_OPEN_API
WMKC_OF((wmkc_obj **dst, wmkc_obj *left, wmkc_obj *right))
{
    wmkcErr_obj error;
    if(!dst || !left || !right) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcStream_Splicing: dst or left or right is NULL.");
    }

    (*dst)->size = (left->size + right->size);
    if(!(*dst)->size) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcStream_Splicing: dst->size is zero.");
    }

    wmkcObject_new(dst, (left->size + right->size) + 1, true);

    memcpy((*dst)->buf, left->buf, left->size);
    memcpy((*dst)->buf + left->size, right->buf, right->size);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcStream_Slice WMKC_OPEN_API
WMKC_OF((wmkc_obj **dst, wmkc_obj *src, wmkcSSize start, wmkcSSize end))
{
    wmkcErr_obj error;
    if(end < 0) {
        if((end = src->size + end) > src->size) {
            wmkcErr_return(error, wmkcErr_ErrOutRange,
                "wmkcStream_Slice: The range at the end is too large.");
        }
    } else if(end > src->size) {
        wmkcErr_return(error, wmkcErr_ErrOutRange,
            "wmkcStream_Slice: The range at the end is too large.");
    }
    if(start < 0) {
        if((start = src->size + start) > src->size) {
            wmkcErr_return(error, wmkcErr_ErrOutRange,
                "wmkcStream_Slice: The range at the start is too large.");
        }
    } else if(start > src->size) {
        wmkcErr_return(error, wmkcErr_ErrOutRange,
            "wmkcStream_Slice: The range at the start is too large.");
    }
    if(start > end) {
        if(end) {
        wmkcErr_return(error, wmkcErr_ErrInvalidRange,
            "wmkcStream_Slice: The starting subscript exceeds the ending subscript.");
        } else {
            end = src->size;
        }
    }

    // printf("size: %lld\n", src->size);
    // printf("start: %lld\n", start);
    // printf("end: %lld\n", end);

    wmkcObject_new(dst, (end - start) + 1, false);
    (*dst)->buf[end - start] = 0x00;
    (*dst)->size = (end - start);

    memcpy((*dst)->buf, src->buf + start, (*dst)->size);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcStream_Scanf WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize size))
{
    wmkcErr_obj error;
    if(!buf || !size) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcStream_Scanf: buf or size is NULL.");
    }

    for(wmkcSize x = 0; x < size; ++x) {
        if((buf[x] = getchar()) == 0x0a) {
            buf[x] = 0x00;
            break;
        }
    }

    if(!(*buf)) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcStream_Scanf: buf is NULL.");
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcStream_Reverse WMKC_OPEN_API
WMKC_OF((wmkcByte *data, wmkcSize size))
{
    wmkcErr_obj error;
    if(!data || !size) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcStream_Reverse: buf or size is NULL.");
    }
    wmkcSize i;
    wmkcByte buf;
    for(i = 0; i < size / 2; ++i) {
        buf = data[i];
        data[i] = data[size - i - 1];
        data[size - i - 1] = buf;
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcStream_Binary WMKC_OPEN_API
WMKC_OF((wmkcByte **dst, wmkcSize src))
{
    wmkcErr_obj error;
    if(!dst || !src) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcStream_Binary: dst or src is NULL.");
    }
    wmkcSize temp;
    wmkcSize size;
    wmkcSize i;

    for(temp = src, size = 0; temp; size += 8, temp >>= 8) {}

    if(!wmkcMemoryNew(wmkcByte *, (*dst), size + 1)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcStream_Binary: (*dst) Failed to apply for memory.");
    }
    (*dst)[size] = 0x00;
    memset((*dst), 0x30, size);

    for(i = 0; src; src >>= 1, ++i) {
        (*dst)[i] = (src & 0x1) + 0x30;
    }

    wmkcStream_Reverse((*dst), size);
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcBool) wmkcStream_Compare WMKC_OPEN_API
WMKC_OF((wmkcByte *src1, wmkcByte *src2, wmkcSize size))
{
    if(!src1 || !src2 || size) {
        return false;
    }
    if(!strncmp((wmkcChar *)src1, (wmkcChar *)src2, size)) {
        return true;
    } else {
        return false;
    }
}

WMKC_PUBLIC(wmkc_u32) wmkcStream_numlen WMKC_OPEN_API
WMKC_OF((wmkcSize _n))
{
    wmkcSize len = 0;
    for(wmkcSize num = _n; num > 0; ++len) num /= 10;
    return len;
}
