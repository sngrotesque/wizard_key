#include <wmkc_bigNum.h>

/**
 * @brief 大数反转函数
 * @authors SN-Grotesque
 * 
 * 此函数将wmkcChar字符串指针进行反转。
 * 
 * @note 私有函数。
 * @param number_string 此参数是一个wmkcChar指针，指向数字字符串的地址。
 * @param size 此参数是number_string的长度。
 * @return 返回一个wmkcErr_obj对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PRIVATE(wmkcErr_obj) wmkcBigNum_ArrayReversal
WMKC_OF((wmkcChar *number_string, wmkcSize size))
{
    wmkcErr_obj error;
    if(!number_string || !size) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcBigNum_ArrayReversal: number_string or size is NULL.");
    }
    wmkcSize i;
    wmkcByte swap;

    for(i = 0; i < size >> 1; ++i) {
        swap = number_string[i];
        number_string[i] = number_string[size - i - 1];
        number_string[size - i - 1] = swap;
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

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
 * @return 返回一个wmkcErr_obj对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_add WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *src1, wmkcChar *src2))
{
    /**
     * @example
     * larger  = "1234561092843327530284087432857309150242358"
     * smaller = "9583209419874982749813573892749837"
     * res = {}
     * 
     * blockSize = 8
     * 
     * larger  -> Reversal -> 8532420519037582347804820357233482901654321
     * smaller -> Reversal -> 7389472983753189472894789149023859
     * 
     * larger  -> Padding  -> 853242051903758234780482035723348290165432100000
     * smaller -> Padding  -> 738947298375318947289478914902385900000000000000
     * 
     * larger  -> Reversal -> 000001234561092843327530284087432857309150242358
     * smaller -> Reversal -> 000000000000009583209419874982749813573892749837
     * 
     * larger  -> block    -> {     123, 45610928, 43327530, 28408743, 28573091, 50242358}
     * smaller -> block    -> {       0,       95, 83209419, 87498274, 98135738, 92749837}
     * 
     * larger  -> Reversal -> {50242358, 28573091, 28408743, 43327530, 45610928,      123}
     * smaller -> Reversal -> {92749837, 98135738, 87498274, 83209419,       95,        0}
     * res -> init     -> {       0,        0,        0,        0,        0,        0}
     * 
     * for(i = 0; i < len(larger); ++i) {
     *     res[i] += (larger[i] + smaller[i]);
     *     if(res[i] >= 100000000) {
     *         res[i] %= res[i];
     *         res[i + 1] += 1;
     *     }
     * }
     * 
     * res ->             {42992195, 26708830, 15907018, 26536950, 45611024,      123}
     * res -> Reversal -> {     123, 45611024, 26536950, 15907018, 26708830, 42992195}
     * res -> String   -> "1234561102426536950159070182670883042992195"
    */
    wmkcErr_obj error;
    if(!dst || !src1 || !src2) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcBigNum_add: dst or src1 or src2 is NULL.");
    }

    wmkcSize src1Size = strlen(src1);
    wmkcSize src2Size = strlen(src2);
    if(!src1Size || !src2Size) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcBigNum_add: src1Size or src2Size is NULL.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

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
 * @return 返回一个wmkcErr_obj对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_sub WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *src1, wmkcChar *src2))
{
    wmkcErr_obj error;
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

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
 * @return 返回一个wmkcErr_obj对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_mul WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *src1, wmkcChar *src2))
{
    /**
     * 可参考wmkcBigNum_add。
    */
    wmkcErr_obj error;
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

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
 * @return 返回一个wmkcErr_obj对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_div WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *src1, wmkcChar *src2))
{
    wmkcErr_obj error;
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

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
 * @return 返回一个wmkcErr_obj对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_mod WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *src1, wmkcChar *src2))
{
    wmkcErr_obj error;
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

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
 * @return 返回一个wmkcErr_obj对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBigNum_pow WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *src1, wmkcChar *src2))
{
    wmkcErr_obj error;
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
