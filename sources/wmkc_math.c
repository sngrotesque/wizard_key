#include <wmkc_math.h>

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
WMKC_OF((wmkcSize n, wmkcSize m))
{
    wmkcSize buf, i;
    if(!m) return 1;
    for(i = 1, buf = n; i < m; ++i, buf *= n);
    return buf;
}

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
WMKC_OF((wmkcSize min, wmkcSize max))
{
	wmkcFast wmkcSize x;
	wmkcFast wmkcSize i;
    wmkcFast wmkcSize count;
    wmkcSize *p;

    if(min % 2 == 0 && min > 1)
        min += 1;

    if(min < 3) {
        p = (wmkcSize *)malloc(2 * sizeof(wmkcSize));
        *(p + 0) = 1;
        *(p + 1) = 2;
    }

    p = (wmkcSize *)malloc(sizeof(wmkcSize));
    count = 0;

	for(x = min; x <= max; x += 2) {
		for(i = 2; i * i <= x; i++)
			if(x % i == 0)
				break;
		if(i * i > x) {
            *(p + count) = x;
            p = (wmkcSize *)realloc(p, sizeof(wmkcSize) * (count + 2));
            ++count;
        }
	}

    *(p + count) = 0;
    return p;
}

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
WMKC_OF((wmkcSize n1, wmkcSize n2))
{
    wmkcSize total;
    wmkcSize n;
    wmkcSize count;
    wmkcSize *p;

    p = (wmkcSize *)malloc(sizeof(wmkcSize));

    if(n1 > n2)
        total = n2;
    else
        total = n1;

    for(count = 0, n = 2; n < total; ++n) {
        if(!((n1 % n) | (n2 % n))) {
            *(p + count) = n;
            p = (wmkcSize *)realloc(p, sizeof(wmkcSize) * (count + 2));
            ++count;
        }
    }

    *(p + count) = 0;
    return p;
}

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
WMKC_OF((wmkcSize n))
{
    wmkcSize tmp;
    wmkcSize count;
    wmkcSize *p;

    p = (wmkcSize *)malloc(sizeof(wmkcSize));

    for(count = 0, tmp = 2; tmp < n; ++tmp) {
        if(!(n % tmp)) {
            *(p + count) = tmp;
            p = (wmkcSize *)realloc(p, (sizeof(wmkcSize) * (count + 2)));
            ++count;
        }
    }

    *(p + count) = 0;
    return p;
}
