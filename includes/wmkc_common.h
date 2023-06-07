#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_COMMON

/**
 * @brief 用于将缓冲区内的数据反转
 * @authors SN-Grotesque
 * @note 这个函数暂时不能处理浮点数数组，后续进行优化
 * @param buf 这是一个指针，指向缓冲区的地址
 * @param size 这是一个整数，代表缓冲区中的内容的长度
 * @param elementSize 这是一个整数，代表缓冲区中单个元素占用内存的大小
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkc_reverse WMKC_OPEN_API
WMKC_OF((wmkcVoid *buf, wmkcSize size, wmkc_u32 elementSize));

/**
 * @brief 此函数用于替换scanf函数
 * @authors SN-Grotesque
 * 
 * 此函数内容与scanf函数类型，但更加安全。
 * 
 * @note 无
 * @param buf 这是一个指针，指向缓冲区的地址。
 * @param size 这是一个指针，代表需要输入至缓冲区的内容长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkc_scanf WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize size));

/**
 * @brief 去掉一个字符串两边的指定字符
 * @authors SN-Grotesque
 * @note 无
 * @param dst 这是一个指针，指向结果指针的地址
 * @param src 这是一个指针，指向源内容的地址
 * @param symbol 这是一个字符，表示去掉去掉的字符
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PRIVATE(wmkcErr_obj) wmkc_trim WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *src, wmkcChar symbol));


#endif /* WMKC_COMMON */
#endif /* WMKC_SUPPORT */
