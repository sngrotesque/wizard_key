#include <wmkc_binascii.h>

// sn_a2b_hex函数的解码表
WMKC_PRIVATE_CONST(wmkcByte) _wmkc_a2b_hexTable[256] = {
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

WMKC_PRIVATE(wmkcByte) _wmkcBinasciiToTop
WMKC_OF((wmkcByte c))
{
    if((c >> 4) < 0x0a)
        return (c >> 4) + 0x30;
    else
        return (c >> 4) + 0x57;
}

WMKC_PRIVATE(wmkcByte) _wmkcBinasciiToBot
WMKC_OF((wmkcByte c))
{
    if((c & 0x0f) < 0x0a)
        return (c & 0x0f) + 0x30;
    else
        return (c & 0x0f) + 0x57;
}

WMKC_PUBLIC(wmkcErr_ctx) wmkcBinascii_b2a_hex WMKC_OPEN_API
WMKC_OF((wmkcByte **dst, wmkcByte *src, wmkcSize srcSize))
{
    wmkcErr_ctx error;
    if(!dst || !src || !srcSize) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcBinascii_b2a_hex: dst or src or srcSize is NULL.");
    }
    wmkcFast wmkcSize i;

    if(!wmkcMemoryNew(wmkcByte *, (*dst), (srcSize << 1) + 1)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcBinascii_b2a_hex: (*dst) Failed to apply for memory.");
    }

    (*dst)[srcSize << 1] = 0x00;

    for(i = 0; i < srcSize; ++i) {
        (*dst)[i<<1]     = _wmkcBinasciiToTop(src[i]);
        (*dst)[(i<<1)+1] = _wmkcBinasciiToBot(src[i]);
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_ctx) wmkcBinascii_a2b_hex WMKC_OPEN_API
WMKC_OF((wmkcByte **dst, wmkcByte *src, wmkcSize srcSize))
{
    wmkcErr_ctx error;
    if(!dst || !src || !srcSize) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcBinascii_a2b_hex: dst or src or srcSize is NULL.");
    }
    wmkcSize src_i, dst_i;
    wmkc_s32 top, bot;

    if(srcSize % 2) {
        wmkcErr_return(error, wmkcErr_ErrType,
            "wmkcBinascii_a2b_hex: Wrong type, should not be an odd length.");
    }

    if(!wmkcMemoryNew(wmkcByte *, (*dst), (srcSize >> 1) + 1)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcBinascii_a2b_hex: (*dst) Failed to apply for memory.");
    }
    (*dst)[(srcSize >> 1)] = 0x00;

    for(src_i = dst_i = 0; src_i < srcSize; src_i += 2, ++dst_i) {
        top = _wmkc_a2b_hexTable[src[src_i]];
        bot = _wmkc_a2b_hexTable[src[src_i+1]];
        if((top | bot) > 30) {
            wmkcErr_return(error, wmkcErr_ErrType,
                "wmkcBinascii_a2b_hex: Wrong type, characters must be from 0 to f.");
        }
        (*dst)[dst_i] = (top << 4) + bot;
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}