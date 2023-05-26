#ifndef WMKC_BIG_NUMBER
#define WMKC_BIG_NUMBER
#include <wmkc_conf.h>

/**
 *  大数加法
 *      目前暂时只支持两个数都为正整数的情况下相加（不支持浮点数与负数）。
 *      这个函数出于性能考虑不会检查你的数组是否只含有数字。
 *  目前的构思：
 *      两个都为负数的情况和单个为负数的情况，等于大的数减去小的数。
 *      如果是一个正数一个负数的情况，那么为大的数减去小的数的相反数。
 *      列子：
 *            (-1) +   (-1) ==   (-1) -   1 =   -2
 *          (-123) +  (-13) == (-123) -  13 = -136
 *          (-123) + (-234) == (-123) - 234 = -357
 *          (-123) +   234  ==    234 - 123 =  111
 *          (-123) +   111  ==    111 - 123 =  -12
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_add WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *_src1, wmkcChar *_src2));

/**
 *  大数减法
 *  
 *  关于减法，这是一个列子：
 *      256 - 768 = -512
 *      768 - 256 = 512
 *  
 *  如果a - b等于负数，那么b - a等于这个负数的相反数。
 *  数字结果长度的话：
 *      如果需要取前一位的值，那么就是最大的数的长度减一。
 *      否则长度等于最大的数的长度。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_sub WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *_src1, wmkcChar *_src2));

/**
 *  关于乘法
 *  如果两个数都小于4，那么结果的长度等于1。
 *  否则，结果的长度等于这两个数的长度相加（有时会是两数长度相加后减一）。
 *  这是一个例子：
 *      ( 1,  1,  1)             3 *             3 =                          9
 *      ( 6,  5, 10)        315984 *         10923 =                 3451493232
 *      (13, 13, 26) 3598209485189 * 5389829038094 = 19393733968416934809789766
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_mul WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *_src1, wmkcChar *_src2));

/**
 *  关于除法
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_div WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *_src1, wmkcChar *_src2));

/**
 *  关于模运算
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_mod WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *_src1, wmkcChar *_src2));

/**
 *  关于次方
 *  任何数的一次方都等于它本身，任何数的零次方都等于1。
 *  0和1的任何次方都等于它们本身。
 *  
 *  暂时并没有想到该如何计算次方的结果长度
 *  
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_pow WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *_src1, wmkcChar *_src2));

#endif
