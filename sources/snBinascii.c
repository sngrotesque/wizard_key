#include <snBinascii.h>

// sn_a2b_hex函数的解码表
SN_PRIVATE_CONST(snByte) _a2b_hex_table[256] = {
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    0,   1,  2,  3,  4,  5,  6,  7,  8,  9, 31, 31, 31, 31, 31, 31,
    31, 10, 11, 12, 13, 14, 15, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 10, 11, 12, 13, 14, 15, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31
};

SN_PRIVATE(snByte) _sn_binascii_to_top
SN_FUNC_OF((snByte c))
{
    if((c >> 4) < 0x0a)
        return (c >> 4) + 0x30;
    else
        return (c >> 4) + 0x57;
}

SN_PRIVATE(snByte) _sn_binascii_to_bot
SN_FUNC_OF((snByte c))
{
    if((c & 0x0f) < 0x0a)
        return (c & 0x0f) + 0x30;
    else
        return (c & 0x0f) + 0x57;
}

SN_PUBLIC(snErr_ctx) snBinascii_b2a_hex SN_OPEN_API
SN_FUNC_OF((snByte **dst, snByte *src, snSize srcSize))
{
    snErr_ctx error;

    if(!dst || !src || !srcSize) {
        snErr_return(error, snErr_ErrNullData,
            "snBinascii_b2a_hex: dst or src or srcSize is NULL.");
    }
    snFast snSize i;

    if(!snMemoryNew(snByte *, (*dst), (srcSize << 1) + 1)) {
        snErr_return(error, snErr_ErrMemory,
            "snBinascii_b2a_hex: (*dst) Failed to apply for memory.");
    }

    (*dst)[srcSize << 1] = 0x00;

    for(i = 0; i < srcSize; ++i) {
        (*dst)[i<<1]     = _sn_binascii_to_top(src[i]);
        (*dst)[(i<<1)+1] = _sn_binascii_to_bot(src[i]);
    }

    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snErr_ctx) snBinascii_a2b_hex SN_OPEN_API
SN_FUNC_OF((snByte **dst, snByte *src, snSize srcSize))
{
    snErr_ctx error;

    if(!dst || !src || !srcSize) {
        snErr_return(error, snErr_ErrNullData,
            "snBinascii_a2b_hex: dst or src or srcSize is NULL.");
    }
    static snSize src_i, dst_i;
    static sn_32 top, bot;

    if (srcSize % 2) {
        snErr_return(error, snErr_ErrType,
            "snBinascii_a2b_hex: Wrong type, should not be an odd length.");
    }

    if(!snMemoryNew(snByte *, (*dst), (srcSize >> 1) + 1)) {
        snErr_return(error, snErr_ErrMemory,
            "snBinascii_a2b_hex: (*dst) Failed to apply for memory.");
    }
    (*dst)[(srcSize >> 1)] = 0x00;

    for (src_i = dst_i = 0; src_i < srcSize; src_i += 2, ++dst_i) {
        top = _a2b_hex_table[src[src_i]];
        bot = _a2b_hex_table[src[src_i+1]];
        if((top | bot) > 30) {
            snErr_return(error, snErr_ErrType,
                "snBinascii_a2b_hex: Wrong type, characters must be from 0 to f.");
        }
        (*dst)[dst_i] = (top << 4) + bot;
    }

    snErr_return(error, snErr_OK, "OK.");
}