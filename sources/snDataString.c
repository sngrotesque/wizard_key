#include <snDataString.h>

WMKC_PUBLIC(snErr_ctx) snString_Splicing WMKC_OPEN_API
WMKC_OF((snObject **dst, snObject *left, snObject *right))
{
    snErr_ctx error;
    if(!dst || !left || !right) {
        snErr_return(error, snErr_ErrNULL,
            "snString_Splicing: dst or left or right is NULL.");
    }

    (*dst)->size = (left->size + right->size);
    if(!(*dst)->size) {
        snErr_return(error, snErr_ErrNULL, "snString_Splicing: dst->size is zero.");
    }

    snObject_new(dst, (left->size + right->size) + 1, true);

    memcpy((*dst)->buf, left->buf, left->size);
    memcpy((*dst)->buf + left->size, right->buf, right->size);

    snErr_return(error, snErr_OK, "OK.");
}

WMKC_PUBLIC(snErr_ctx) snString_Slice WMKC_OPEN_API
WMKC_OF((snObject **dst, snObject *src, wmkcSSize start, wmkcSSize end))
{
    snErr_ctx error;
    if(end < 0) {
        if((end = src->size + end) > src->size) {
            snErr_return(error, snErr_ErrOutRange,
                "snString_Slice: The range at the end is too large.");
        }
    } else if(end > src->size) {
        snErr_return(error, snErr_ErrOutRange,
            "snString_Slice: The range at the end is too large.");
    }
    if(start < 0) {
        if((start = src->size + start) > src->size) {
            snErr_return(error, snErr_ErrOutRange,
                "snString_Slice: The range at the start is too large.");
        }
    } else if(start > src->size) {
        snErr_return(error, snErr_ErrOutRange,
            "snString_Slice: The range at the start is too large.");
    }
    if(start > end) {
        if(end) {
        snErr_return(error, snErr_ErrInvalidRange,
            "snString_Slice: The starting subscript exceeds the ending subscript.");
        } else {
            end = src->size;
        }
    }

    // printf("size: %lld\n", src->size);
    // printf("start: %lld\n", start);
    // printf("end: %lld\n", end);

    snObject_new(dst, (end - start) + 1, false);
    (*dst)->buf[end - start] = 0x00;
    (*dst)->size = (end - start);

    memcpy((*dst)->buf, src->buf + start, (*dst)->size);

    snErr_return(error, snErr_OK, "OK.");
}

WMKC_PUBLIC(snErr_ctx) snString_Scanf WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize size))
{
    snErr_ctx error;
    if(!buf || !size) {
        snErr_return(error, snErr_ErrNULL, "snString_Scanf: buf or size is NULL.");
    }

    for(wmkcSize x = 0; x < size; ++x) {
        if((buf[x] = getchar()) == 0x0a) {
            buf[x] = 0x00;
            break;
        }
    }

    if(!(*buf)) {
        snErr_return(error, snErr_ErrNULL, "snString_Scanf: buf is NULL.");
    }
    snErr_return(error, snErr_OK, "OK.");
}

WMKC_PUBLIC(snErr_ctx) snString_Reverse WMKC_OPEN_API
WMKC_OF((wmkcByte *data, wmkcSize size))
{
    snErr_ctx error;
    if(!data || !size) {
        snErr_return(error, snErr_ErrNULL, "snString_Reverse: buf or size is NULL.");
    }
    wmkcSize i;
    wmkcByte buf;
    for(i = 0; i < size / 2; ++i) {
        buf = data[i];
        data[i] = data[size - i - 1];
        data[size - i - 1] = buf;
    }
    snErr_return(error, snErr_OK, "OK.");
}

WMKC_PUBLIC(snErr_ctx) snString_Binary WMKC_OPEN_API
WMKC_OF((wmkcByte **dst, wmkcSize src))
{
    snErr_ctx error;
    if(!dst || !src) {
        snErr_return(error, snErr_ErrNULL, "snString_Binary: dst or src is NULL.");
    }
    wmkcSize temp;
    wmkcSize size;
    wmkcSize i;

    for(temp = src, size = 0; temp; size += 8, temp >>= 8) {}

    if(!wmkcMemoryNew(wmkcByte *, (*dst), size + 1)) {
        snErr_return(error, snErr_ErrMemory,
            "snString_Binary: (*dst) Failed to apply for memory.");
    }
    (*dst)[size] = 0x00;
    memset((*dst), 0x30, size);

    for(i = 0; src; src >>= 1, ++i) {
        (*dst)[i] = (src & 0x1) + 0x30;
    }

    snString_Reverse((*dst), size);
    snErr_return(error, snErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcBool) snString_Compare WMKC_OPEN_API
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

WMKC_PUBLIC(wmkc_u32) snString_numlen WMKC_OPEN_API
WMKC_OF((wmkcSize _n))
{
    wmkcSize len = 0;
    for(wmkcSize num = _n; num > 0; ++len) num /= 10;
    return len;
}
