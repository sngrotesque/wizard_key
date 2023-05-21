#include <snBase64.h>

SN_PRIVATE_CONST(snByte) _B64ET[64] = { // Base64编码表
    65,   66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,
    79,   80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  97,  98,
    99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
    113, 114, 115, 116, 117, 118, 119, 120, 121, 122,  48,  49,  50,  51,
    52,   53,  54,  55,  56,  57,  43,  47};
SN_PRIVATE_CONST(snByte) _B64DT[123] = { // Base64解码表
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  62, 0,  0,  0,  63, 52, 53, 54, 55, 56, 57,
    58, 59, 60, 61, 0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,
    7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 0,  0,  0,  0,  0,  0,  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
    37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};

SN_PUBLIC(snSize) snB64E_size SN_OPEN_API
SN_FUNC_OF((snObject *src))
{
    if(src->size % 3)
        return (src->size / 3 + 1) * 4;
    return src->size / 3 * 4;
}

SN_PUBLIC(snSize) snB64D_size SN_OPEN_API
SN_FUNC_OF((snObject *src))
{
    if ((src->buf[src->size - 1] & src->buf[src->size - 2]) == BASE64_PAD)
        return src->size / 4 * 3 - 2;
    else if (src->buf[src->size - 1] == BASE64_PAD)
        return src->size / 4 * 3 - 1;
    else
        return src->size / 4 * 3;
}

SN_PUBLIC(snErr_ctx *) snBase64_new SN_OPEN_API
SN_FUNC_OF((snBase64_ctx **obj))
{
    snErr_ctx *error = snNull;
    snErr_new(error);

    if(!obj) {
        snErr_return(error, snErr_ErrNullData, "obj is NULL.");
    }
    if(!snMemoryNew(snBase64_ctx *, (*obj), sizeof(snBase64_ctx))) {
        snErr_return(error, snErr_ErrMemory, "(*obj) Failed to apply for memory.");
    }
    if(!snMemoryNew(snObject *, (*obj)->src, sizeof(snObject))) {
        snErr_return(error, snErr_ErrMemory, "(*obj)->src Failed to apply for memory.");
    }
    if(!snMemoryNew(snObject *, (*obj)->dst, sizeof(snObject))) {
        snErr_return(error, snErr_ErrMemory, "(*obj)->dst Failed to apply for memory.");
    }

    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snErr_ctx *) snBase64_free SN_OPEN_API
SN_FUNC_OF((snBase64_ctx **obj))
{
    snErr_ctx *error = snNull;
    snErr_new(error);

    if(!obj) {
        snErr_return(error, snErr_ErrNullData, "obj is NULL.");
    }
    snMemoryFree((*obj)->src);
    snMemoryFree((*obj)->dst);
    snMemoryFree((*obj));

    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snErr_ctx *) snBase64Encode SN_OPEN_API
SN_FUNC_OF((snObject *dst, snObject *src))
{
    snErr_ctx *error = snNull;
    snErr_new(error);
    if(!dst || !src) {
        snErr_return(error, snErr_ErrNullData, "dst or src is null.");
    }
    if(!dst->buf || !src->buf) {
        snErr_return(error, snErr_ErrNullData, "dst->buf or src->buf is null.");
    }
    if(!dst->size || !src->size) {
        snErr_return(error, snErr_ErrNullData, "dst->size or src->size is null.");
    }
    snSize dst_i, src_i;

    for (dst_i = src_i = 0; dst_i < dst->size - 2; src_i += 3, dst_i += 4) {
        dst->buf[dst_i]   = _B64ET[src->buf[src_i]           >> 2];
        dst->buf[dst_i+1] = _B64ET[(src->buf[src_i]   & 0x3) << 4 | (src->buf[src_i+1] >> 4)];
        dst->buf[dst_i+2] = _B64ET[(src->buf[src_i+1] & 0xf) << 2 | (src->buf[src_i+2] >> 6)];
        dst->buf[dst_i+3] = _B64ET[src->buf[src_i+2]  & 0x3f];
    }

    switch(src->size % 3) {
        case 1:
            dst->buf[dst_i - 2] = BASE64_PAD;
            dst->buf[dst_i - 1] = BASE64_PAD;
            break;
        case 2:
            dst->buf[dst_i - 1] = BASE64_PAD;
            break;
    }

    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snErr_ctx *) snBase64Decode SN_OPEN_API
SN_FUNC_OF((snObject *dst, snObject *src))
{
    snErr_ctx *error = snNull;
    snErr_new(error);
    if(!dst || !src) {
        snErr_return(error, snErr_ErrNullData, "dst or src is null.");
    }
    if(!dst->buf || !src->buf) {
        snErr_return(error, snErr_ErrNullData, "dst->buf or src->buf is null.");
    }
    if(!dst->size || !src->size) {
        snErr_return(error, snErr_ErrNullData, "dst->size or src->size is null.");
    }
    snSize src_i, dst_i;

    for (src_i = dst_i = 0; src_i < src->size - 2; dst_i += 3, src_i += 4) {
        dst->buf[dst_i]   = (_B64DT[src->buf[src_i]]   << 2) |  (_B64DT[src->buf[src_i+1]]  >> 4);
        dst->buf[dst_i+1] = (_B64DT[src->buf[src_i+1]] << 4) | ((_B64DT[src->buf[src_i+2]]) >> 2);
        dst->buf[dst_i+2] = (_B64DT[src->buf[src_i+2]] << 6) |   _B64DT[src->buf[src_i+3]];
    }

    snErr_return(error, snErr_OK, "OK.");
}
