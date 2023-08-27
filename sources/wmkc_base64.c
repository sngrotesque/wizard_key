#include <wmkc_base64.h>

// Base64编码表
WMKC_PRIVATE_CONST(wmkcByte) _B64ET[64] = {
    65,   66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77, 78,  79,   80,
    81,   82,  83,  84,  85,  86,  87,  88,  89,  90,  97,  98,  99, 100, 101, 102,
    103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118,
    119, 120, 121, 122,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  43,  47};

// Base64解码表
WMKC_PRIVATE_CONST(wmkcByte) _B64DT[123] = {
    0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,   0,  0,  62, 0,  0,  0, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,  0,  0,
    0,   0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,  0,  0,  0,  0,  0, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
    49, 50, 51};

/**
 * @brief 获取数据经过Base64编码后的长度
 * @authors SN-Grotesque
 * @note 无。
 * @param size 此参数是一个源内容的长度，通过计算得到编码后的长度。
 * @return 此函数返回一个wmkcSize类型变量，代表编码后的长度值。
*/
WMKC_PRIVATE(wmkcSize) _wmkcBase64_encodeSize WMKC_OPEN_API
WMKC_OF((wmkcSize size))
{
    return (size % 3) ? ((size / 3 + 1) * 4) : (size / 3 * 4);
}

/**
 * @brief 获取数据经过Base64解码后的长度
 * @authors SN-Grotesque
 * @note 无。
 * @param src 此参数是一个指针，指向源内容缓冲区的地址。
 * @param size 此参数是一个源内容的长度，通过计算得到解码后的长度。
 * @return 此函数返回一个wmkcSize类型变量，代表解码后的长度值。
*/
WMKC_PRIVATE(wmkcSize) _wmkcBase64_decodeSize WMKC_OPEN_API
WMKC_OF((wmkcByte *src, wmkcSize size))
{
    wmkcSize decodedSize = size / 4 * 3;
    if(src[size - 1] == BASE64_PAD)
        --decodedSize;
    if(src[size - 2] == BASE64_PAD)
        --decodedSize;
    return decodedSize;
}

/**
 * @brief Base64编码函数
 * @authors SN-Grotesque
 * @note 无
 * @param dst 指针，指向结果指针的地址。
 * @param src 指针，指向源内容的地址。
 * @param srcSize 这是一个长度，代表源内容的长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcBase64_encode WMKC_OPEN_API
WMKC_OF((wmkcByte **dst, wmkcByte *src, wmkcSize srcSize))
{
    wmkcErr_obj error;
    if(!dst || !src || !srcSize) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcBase64_encode",
            "dst or src or srcSize is null.");
    }
    wmkcSize dst_i, src_i, dstSize;

    dstSize = _wmkcBase64_encodeSize(srcSize);
    if(!wmkcMem_new(wmkcByte *, (*dst), dstSize + 1)) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcBase64_encode",
            "Failed to allocate memory for (*dst).");
    }
    (*dst)[dstSize] = 0x00;

    for (dst_i = src_i = 0; dst_i < dstSize - 2; src_i += 3, dst_i += 4) {
        (*dst)[dst_i]   = _B64ET[src[src_i]           >> 2];
        (*dst)[dst_i+1] = _B64ET[(src[src_i]   & 0x3) << 4 | (src[src_i+1] >> 4)];
        (*dst)[dst_i+2] = _B64ET[(src[src_i+1] & 0xf) << 2 | (src[src_i+2] >> 6)];
        (*dst)[dst_i+3] = _B64ET[src[src_i+2]  & 0x3f];
    }

    switch(srcSize % 3) {
        case 1:
            (*dst)[dst_i - 2] = BASE64_PAD;
        case 2:
            (*dst)[dst_i - 1] = BASE64_PAD;
            break;
    }

    wmkcErr_return(error, wmkcErr_OK, "wmkcBase64_encode", "OK.");
}

/**
 * @brief Base64解码函数
 * @authors SN-Grotesque
 * @note 无
 * @param dst 指针，指向结果指针的地址。
 * @param src 指针，指向源内容的地址。
 * @param srcSize 这是一个长度，代表源内容的长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcBase64_decode WMKC_OPEN_API
WMKC_OF((wmkcByte **dst, wmkcByte *src, wmkcSize srcSize))
{
    wmkcErr_obj error;
    if(!dst || !src || !srcSize) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcBase64_decode",
            "dst or src or srcSize is null.");
    }
    if(srcSize & 3) {
        wmkcErr_return(error, wmkcErr_ErrType, "wmkcBase64_decode",
            "The length of the src is incorrect.");
    }
    wmkcSize dst_i, src_i, dstSize;

    dstSize = _wmkcBase64_decodeSize(src, srcSize);
    if(!wmkcMem_new(wmkcByte *, (*dst), dstSize + 1)) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcBase64_decode",
            "Failed to allocate memory for (*dst).");
    }
    (*dst)[dstSize] = 0x00;

    for (src_i = dst_i = 0; src_i < srcSize - 2; dst_i += 3, src_i += 4) {
        (*dst)[dst_i]   = (_B64DT[src[src_i]]   << 2) |  (_B64DT[src[src_i+1]]  >> 4);
        (*dst)[dst_i+1] = (_B64DT[src[src_i+1]] << 4) | ((_B64DT[src[src_i+2]]) >> 2);
        (*dst)[dst_i+2] = (_B64DT[src[src_i+2]] << 6) |   _B64DT[src[src_i+3]];
    }

    wmkcErr_return(error, wmkcErr_OK, "wmkcBase64_decode", "OK.");
}
