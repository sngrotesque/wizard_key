#include <snDataString.h>

SN_PUBLIC(snError) snString_Splicing SN_OPEN_API
SN_FUNC_OF((snObject **dst, snObject *left, snObject *right))
{
    snObject_new(dst, (left->size + right->size) + 1, true);
    (*dst)->size = (left->size + right->size);

    if(!(*dst)->size) {
        return snErr_ErrNullData;
    }

    memcpy((*dst)->buf, left->buf, left->size);
    memcpy((*dst)->buf + left->size, right->buf, right->size);

    return snErr_OK;
}

SN_PUBLIC(snError) snString_Slice SN_OPEN_API
SN_FUNC_OF((snObject **dst, snObject *src, snSSize start, snSSize end))
{
    if(end < 0) {
        if((end = src->size + end) > src->size) {
            return snErr_ErrOutRange;
        }
    } else if(end > src->size) {
        return snErr_ErrOutRange;
    }
    if(start < 0) {
        if((start = src->size + start) > src->size) {
            return snErr_ErrOutRange;
        }
    } else if(start > src->size) {
        return snErr_ErrOutRange;
    }
    if(start > end) {
        if(end) {
            return snErr_ErrInvalidRange;
        } else {
            end = src->size;
        }
    }

    printf("size: %lld\n", src->size);
    printf("start: %lld\n", start);
    printf("end: %lld\n", end);

    snObject_new(dst, (end - start) + 1, false);
    (*dst)->buf[end - start] = 0x00;
    (*dst)->size = (end - start);

    memcpy((*dst)->buf, src->buf + start, (*dst)->size);

    return snErr_OK;
}

SN_PUBLIC(snError) snString_Scanf SN_OPEN_API
SN_FUNC_OF((snByte *buf, snSize size))
{
    if(!buf || !size)
        return snErr_ErrInvalid;

    for(snSize x = 0; x < size; ++x) {
        if((buf[x] = getchar()) == 0x0a) {
            buf[x] = 0x00;
            break;
        }
    }

    if(!(*buf)) {
        return snErr_ErrNullData;
    }
    return snErr_OK;
}

SN_PUBLIC(snError) snString_Reverse SN_OPEN_API
SN_FUNC_OF((snByte *data, snSize size))
{
    if(!data || !size) {
        return snErr_ErrNullData;
    }
    static snSize buf, i;
    for(i = 0; i < size / 2; ++i) {
        buf = data[i];
        data[i] = data[size - i - 1];
        data[size - i - 1] = buf;
    }
    return snErr_OK;
}

SN_PUBLIC(snError) snString_Binary SN_OPEN_API
SN_FUNC_OF((snByte **buf, snSize n))
{
    static snSize temp;
    static snSize size;
    static snSize i;

    for(temp = n, size = 0; temp; size += 8, temp >>= 8) {}

    if(!(*buf)) {
        if(!((*buf) = (snByte *)malloc(size + 1))) {
            return snErr_ErrMemory;
        }
    }
    (*buf)[size] = 0x00;
    memset((*buf), 0x30, size);

    for(i = 0; n; n >>= 1, ++i) {
        (*buf)[i] = (n & 0x1) + 0x30;
    }

    snString_Reverse((*buf), size);
    return snErr_OK;
}

SN_PUBLIC(snBool) snString_Compare SN_OPEN_API
SN_FUNC_OF((snByte *src1, snByte *src2, snSize size))
{
    if(!strncmp((snChar *)src1, (snChar *)src2, size)) {
        return true;
    } else {
        return false;
    }
}

SN_PUBLIC(sn_u32) snString_numlen SN_OPEN_API
SN_FUNC_OF((snSize _n))
{
    snSize len = 0;
    for(snSize num = _n; num > 0; ++len) num /= 10;
    return len;
}
