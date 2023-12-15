#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_BINASCII
#define WMKC_BINASCII
#include <wmkc_error.h>
#include <wmkc_memory.h>

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
 * @param dst 指针，指向用于保存结果的指针的地址。
 * @param src 指针，指向源数据的地址。
 * @param srcSize 这是一个长度，表示源数据的长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBinascii_b2a_hex WMKC_OPEN_API
WMKC_OF((wmkcByte **dst, wmkcByte *src, wmkcSize srcSize));

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
 * @param dst 指针，指向用于保存结果的指针的地址。
 * @param src 指针，指向源数据的地址。
 * @param srcSize 这是一个长度，表示源数据的长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBinascii_a2b_hex WMKC_OPEN_API
WMKC_OF((wmkcByte **dst, wmkcByte *src, wmkcSize srcSize));

#endif /* WMKC_BINASCII */
#endif /* WMKC_SUPPORT */
