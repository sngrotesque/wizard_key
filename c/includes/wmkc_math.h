#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_MATH
#define WMKC_MATH
#include <wmkc_memory.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <math.h>
#include <ctype.h>

/**
 * @brief 求出n的m次方
 * @authors SN-Grotesque
 * 
 * 使用for循环计算n的m次方
 * 
 * @note 此函数的存在是为了兼容部分Linux操作系统在
 *       使用pow函数时需要额外链接一下的问题。
 * @param n 是基数
 * @param m 是幂
 * @return 返回一个wmkcSize类型，为结果
*/
WMKC_PUBLIC(wmkcSize) wmkcMath_pow WMKC_OPEN_API
WMKC_OF((wmkcSize n, wmkcSize m));

/**
 * @brief 求范围内的所有质数
 * @authors SN-Grotesque
 * 
 * 求指定范围内的所有质数
 * 
 * @note 此函数可能存在一些隐藏的BUG，请后续处理一下。
 *       以及我认为这个函数后续可以通过链表等结构优化一下结果。
 * @param min 范围的最小值
 * @param max 范围的最大值
 * @return 返回一个wmkcSize指针，指向结果数组的地址。
*/
WMKC_PUBLIC(wmkcSize *) wmkcMath_primeNumber WMKC_OPEN_API
WMKC_OF((wmkcSize min, wmkcSize max));

/**
 * @brief 求n1和n2的所有公因数
 * @authors SN-Grotesque
 * 
 * 求n1和n2的所有公因数
 * 
 * @note 此函数可能存在一些隐藏的BUG，请后续处理一下。
 *       以及我认为这个函数后续可以通过链表等结构优化一下结果。
 * @param n1 第一个数
 * @param n2 第二个数
 * @return 返回一个wmkcSize指针，指向结果数组的地址。
*/
WMKC_PUBLIC(wmkcSize *) wmkcMath_commonFactor WMKC_OPEN_API
WMKC_OF((wmkcSize n1, wmkcSize n2));

/**
 * @brief 求n的所有因数
 * @authors SN-Grotesque
 * 
 * 求n的所有因数
 * 
 * @note 此函数可能存在一些隐藏的BUG，请后续处理一下。
 *       以及我认为这个函数后续可以通过链表等结构优化一下结果。
 * @param n 一个正整数
 * @return 返回一个wmkcSize指针，指向结果数组的地址。
*/
WMKC_PUBLIC(wmkcSize *) wmkcMath_factor WMKC_OPEN_API
WMKC_OF((wmkcSize n));

/**
 * @brief 进制转换（将2~36进制转为十进制）
 * @authors SN-Grotesque
 * @note 无
 * @param content 对应进制的内容
 * @param base 进制单位（2，3，4...36）
 * @return wmkcErr对象code为非0代表出错，需检查。
 */
WMKC_PUBLIC(wmkcSize) wmkcMath_conversion WMKC_OPEN_API
WMKC_OF((wmkcCSTR content, wmkcByte base));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* WMKC_MATH */
#endif /* WMKC_SUPPORT */
