#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_BIG_NUMBER
#define WMKC_BIG_NUMBER

/**
 * @brief 大数加法函数
 * @authors SN-Grotesque
 * 
 * 此函数将两个大数字符串进行分组相加得到结果。
 * 目前的一些构思：
 *      两个都为负数的情况和单个为负数的情况，等于大的数减去小的数。
 *      如果是一个正数一个负数的情况，那么为大的数减去小的数的相反数。
 *      列子：
 *            (-1) +   (-1) ==   (-1) -   1 =   -2
 *          (-123) +  (-13) == (-123) -  13 = -136
 *          (-123) + (-234) == (-123) - 234 = -357
 *          (-123) +   234  ==    234 - 123 =  111
 *          (-123) +   111  ==    111 - 123 =  -12
 * 
 * @note 未完成
 * @example
 *      wmkcChar *dst = wmkcNull;
 *      wmkcBigNum_add(&dst, "123", "456");
 *      dst得到"579"。
 * @param dst 此参数是一个wmkcChar指针的地址，指向结果指针的地址。
 * @param _src1 此参数是wmkcChar指针，指向第一个数字字符串的地址。
 * @param _src2 此参数是wmkcChar指针，指向第二个数字字符串的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_add WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *_src1, wmkcChar *_src2));

/**
 * @brief 大数减法函数
 * @authors SN-Grotesque
 * 
 * 此函数将两个大数字符串进行分组相减得到结果。
 * 关于减法，这是一个列子：
 *      256 - 768 = -512
 *      768 - 256 = 512
 *  
 * 如果a - b等于负数，那么b - a等于这个负数的相反数。
 * 数字结果长度的话：
 *      如果需要取前一位的值，那么就是最大的数的长度减一。
 *      否则长度等于最大的数的长度。
 * 
 * @note 未完成
 * @example
 *      wmkcChar *dst = wmkcNull;
 *      wmkcBigNum_add(&dst, "123", "456");
 *      dst得到"-333"。
 * @param dst 此参数是一个wmkcChar指针的地址，指向结果指针的地址。
 * @param _src1 此参数是wmkcChar指针，指向第一个数字字符串的地址。
 * @param _src2 此参数是wmkcChar指针，指向第二个数字字符串的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_sub WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *_src1, wmkcChar *_src2));

/**
 * @brief 大数乘法函数
 * @authors SN-Grotesque
 * 
 * 此函数将两个大数字符串进行分组相乘得到结果。
 * 如果两个数都小于4，那么结果的长度等于1。
 * 否则，结果的长度等于这两个数的长度相加（有时会是两数长度相加后减一）。
 * 这是一个例子：
 *      ( 1,  1,  1)             3 *             3 =                          9
 *      ( 6,  5, 10)        315984 *         10923 =                 3451493232
 *      (13, 13, 26) 3598209485189 * 5389829038094 = 19393733968416934809789766
 * 
 * @note 未完成
 * @example
 *      wmkcChar *dst = wmkcNull;
 *      wmkcBigNum_add(&dst, "123", "456");
 *      dst得到"56088"。
 * @param dst 此参数是一个wmkcChar指针的地址，指向结果指针的地址。
 * @param _src1 此参数是wmkcChar指针，指向第一个数字字符串的地址。
 * @param _src2 此参数是wmkcChar指针，指向第二个数字字符串的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_mul WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *_src1, wmkcChar *_src2));

/**
 * @brief 大数除法函数
 * @authors SN-Grotesque
 * 
 * 此函数将两个大数字符串进行分组相除得到结果。
 * 
 * @note 未完成
 * @example
 *      wmkcChar *dst = wmkcNull;
 *      wmkcBigNum_add(&dst, "222", "111");
 *      dst得到"2"。
 * @param dst 此参数是一个wmkcChar指针的地址，指向结果指针的地址。
 * @param _src1 此参数是wmkcChar指针，指向第一个数字字符串的地址。
 * @param _src2 此参数是wmkcChar指针，指向第二个数字字符串的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_div WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *_src1, wmkcChar *_src2));

/**
 * @brief 大数模运算函数
 * @authors SN-Grotesque
 * 
 * 此函数将两个大数字符串进行分组模运算得到结果。
 * 
 * @note 未完成
 * @example
 *      wmkcChar *dst = wmkcNull;
 *      wmkcBigNum_add(&dst, "123", "456");
 *      dst得到"123"。
 * @param dst 此参数是一个wmkcChar指针的地址，指向结果指针的地址。
 * @param _src1 此参数是wmkcChar指针，指向第一个数字字符串的地址。
 * @param _src2 此参数是wmkcChar指针，指向第二个数字字符串的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_mod WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *_src1, wmkcChar *_src2));

/**
 * @brief 大数幂函数
 * @authors SN-Grotesque
 * 
 * 任何数的一次方都等于它本身，任何数的零次方都等于1。
 * 0和1的任何次方都等于它们本身。
 * 暂时并没有想到该如何计算次方的结果长度
 * 
 * @note 未完成
 * @example
 *      wmkcChar *dst = wmkcNull;
 *      wmkcBigNum_add(&dst, "123", "456");
 *      dst得到"123"。
 * @param dst 此参数是一个wmkcChar指针的地址，指向结果指针的地址。
 * @param _src1 此参数是wmkcChar指针，指向第一个数字字符串的地址。
 * @param _src2 此参数是wmkcChar指针，指向第二个数字字符串的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_pow WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *_src1, wmkcChar *_src2));

#endif /* WMKC_BIG_NUMBER */
#endif /* WMKC_SUPPORT */
