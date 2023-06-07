#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_STREAM
#define WMKC_STREAM

#include <wmkc_common.h>
#include <wmkc_object.h>

/**
 * @brief 此函数将参数二与参数三拼接
 * @authors SN-Grotesque
 * 
 * 此函数将参数二与参数三拼接，会自动申请内存空间。
 * 
 * @note 无
 * @param dst 这是一个指针，指向结果指针的地址。
 * @param left 这是一个指针，指向左侧内容的wmkc对象的地址。
 * @param right 这是一个指针，指向右侧内容的wmkc对象的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcStream_Splicing WMKC_OPEN_API
WMKC_OF((wmkc_obj **dst, wmkc_obj *left, wmkc_obj *right));

/**
 * @brief 此函数将内容进行切片
 * @authors SN-Grotesque
 * 
 * 此函数将内容进行切片，会自动申请内存空间。
 * 
 * @note 无
 * @param dst 这是一个指针，指向结果指针的地址。
 * @param src 这是一个指针，指向源内容wmkc对象的地址。
 * @param start 这是起始下标，用于标注切片的起始位置。
 * @param end 这是末尾下标，用于标注切片的末尾位置。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcStream_Slice WMKC_OPEN_API
WMKC_OF((wmkc_obj **dst, wmkc_obj *src, wmkcSSize start, wmkcSSize end));

/**
 * @brief 此函数用于求出指定数字的二进制值
 * @authors SN-Grotesque
 * 
 * 此函数用于求出指定数字的二进制值并保存到dst指针指向的地址中。
 * 
 * @note 此函数后续可以考虑一下与大数库结合，或者将源数字类型改为wmkcFloat。
 * @param dst 这是一个指针，指向结果指针的地址。
 * @param src 这是一个数字，为源数字的内容。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcStream_Binary WMKC_OPEN_API
WMKC_OF((wmkcByte **dst, wmkcSize src));

/**
 * @brief 此函数用于判断两个内容是否相同
 * @authors SN-Grotesque
 * 
 * 此函数用于判断两个内容是否相同，内部使用strncmp函数实现。
 * 
 * @note 无
 * @param src1 这是一个指针，指向源数据1。
 * @param src2 这是一个指针，指向源数据2。
 * @param size 这是一个长度，代表源数据的长度（如果长度不同那你还比较什么呢？）
 * @return 返回一个布尔值，为True时两个源数据相同，否则不同。
*/
WMKC_PUBLIC(wmkcBool) wmkcStream_Compare WMKC_OPEN_API
WMKC_OF((wmkcByte *src1, wmkcByte *src2, wmkcSize size));

/**
 * @brief 此函数用于求出一个整数的长度位数
 * @authors SN-Grotesque
 * 
 * 此函数用于求出一个整数的长度位数（通过除法判断）
 * 
 * @note 无
 * @param src 这是源数字。
 * @return 返回一个wmkc_u32值为长度。
*/
WMKC_PUBLIC(wmkc_u32) wmkcStream_numlen WMKC_OPEN_API
WMKC_OF((wmkcSize src));

#endif /* WMKC_STREAM */
#endif /* WMKC_SUPPORT */
