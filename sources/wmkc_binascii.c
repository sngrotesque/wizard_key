#include <wmkc_binascii.h>

// 解码表
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

/**
 * @brief wmkcBinascii_b2a_hex函数的私有函数
 * @authors SN-Grotesque
 * 
 * 此函数返回一个高位的ASCII字符，范围是[0, 9] - [a, f]。
 * 
 * @note 私有函数
 * @param c 这是一个wmkcByte变量，用于取高位。
 * @return 返回一个wmkcByte变量，为c的高位值。
*/
WMKC_PRIVATE(wmkcByte) _wmkcBinasciiToTop
WMKC_OF((wmkcByte c))
{
    if((c >> 4) < 0x0a)
        return (c >> 4) + 0x30;
    else
        return (c >> 4) + 0x57;
}

/**
 * @brief wmkcBinascii_b2a_hex函数的私有函数
 * @authors SN-Grotesque
 * 
 * 此函数返回一个低位的ASCII字符，范围是[0, 9] - [a, f]。
 * 
 * @note 私有函数
 * @param c 这是一个wmkcByte变量，用于取低位。
 * @return 返回一个wmkcByte变量，为c的低位值。
*/
WMKC_PRIVATE(wmkcByte) _wmkcBinasciiToBot
WMKC_OF((wmkcByte c))
{
    if((c & 0x0f) < 0x0a)
        return (c & 0x0f) + 0x30;
    else
        return (c & 0x0f) + 0x57;
}

/**
 * @brief 二进制数据的十六进制表示。
 * @authors SN-Grotesque
 * 
 * 此函数将src指针指向的内容转为十六进制字符串。
 * 
 * @note 无
 * @example
 *      wmkcByte *dst = wmkcNull;
 *      wmkcBinascii_b2a_hex(&dst, (wmkcByte *)"123", 3);
 *      dst -> 313233
 * @param dst 这是一个指针，指向用于保存结果的指针的地址。
 * @param src 这是一个指针，指向源数据的地址。
 * @param srcSize 这是一个长度，表示源数据的长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBinascii_b2a_hex WMKC_OPEN_API
WMKC_OF((wmkcByte **dst, wmkcByte *src, wmkcSize srcSize))
{
    wmkcErr_obj error;
    if(!dst || !src || !srcSize) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcBinascii_b2a_hex: dst or src or srcSize is NULL.");
    }
    wmkcFast wmkcSize i;

    if(!wmkcMemoryNew(wmkcByte *, (*dst), (srcSize << 1) + 1)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcBinascii_b2a_hex: (*dst) failed to apply for memory.");
    }

    (*dst)[srcSize << 1] = 0x00;

    for(i = 0; i < srcSize; ++i) {
        (*dst)[i<<1]     = _wmkcBinasciiToTop(src[i]);
        (*dst)[(i<<1)+1] = _wmkcBinasciiToBot(src[i]);
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 十六进制表示的二进制数据。
 * @authors SN-Grotesque
 * 
 * 此函数将src指针指向的十六进制字符串转为二进制数据。
 * 
 * @note 无
 * @example
 *      wmkcByte *dst = wmkcNull;
 *      wmkcBinascii_b2a_hex(&dst, (wmkcByte *)"313233", 6);
 *      dst -> 123
 * @param dst 这是一个指针，指向用于保存结果的指针的地址。
 * @param src 这是一个指针，指向源数据的地址。
 * @param srcSize 这是一个长度，表示源数据的长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBinascii_a2b_hex WMKC_OPEN_API
WMKC_OF((wmkcByte **dst, wmkcByte *src, wmkcSize srcSize))
{
    wmkcErr_obj error;
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
            "wmkcBinascii_a2b_hex: (*dst) failed to apply for memory.");
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