#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_BASIC
#define WMKC_BASIC

/**
 * @brief 字节交换函数（0x12 -> 0x21）
 * @authors SN-Grotesque
 * @note 无
 * @param x 这是需要交换的内容
 * @return 返回一个wmkcByte类型的结果
 */
WMKC_PUBLIC(wmkcByte) wmkcBasic_byteSwap WMKC_OPEN_API
WMKC_OF((wmkcByte x));

/**
 * @brief 字交换函数（0x1234 -> 0x4321）
 * @authors SN-Grotesque
 * @note 无
 * @param x 这是需要交换的内容
 * @return 返回一个wmkc_u16类型的结果
 */
WMKC_PUBLIC(wmkc_u16) wmkcBasic_wordSwap WMKC_OPEN_API
WMKC_OF((wmkc_u16 x));

/**
 * @brief 长字交换函数（0x12345678 -> 0x87654321）
 * @authors SN-Grotesque
 * @note 无
 * @param x 这是需要交换的内容
 * @return 返回一个wmkc_u16类型的结果
 */
WMKC_PUBLIC(wmkc_u32) wmkcBasic_longSwap WMKC_OPEN_API
WMKC_OF((wmkc_u32 x));

#endif /* WMKC_BASIC */
#endif /* WMKC_SUPPORT */
