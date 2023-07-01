#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_BASE
#define WMKC_BASE
#include <wmkc_error.h>
#include <wmkc_memory.h>

// Base64的填充值
#define BASE64_PAD '='

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
WMKC_OF((wmkcByte **dst, wmkcByte *src, wmkcSize srcSize));

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
WMKC_OF((wmkcByte **dst, wmkcByte *src, wmkcSize srcSize));

#endif /* WMKC_BASE */
#endif /* WMKC_SUPPORT */
