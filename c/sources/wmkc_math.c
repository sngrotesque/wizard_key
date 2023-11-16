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

/**
 * @brief 进制转换（将2~36进制转为十进制）
 * @authors SN-Grotesque
 * @note 此函数最大可以转换36进制的："3W5E11264SGSF"
 * @param content 对应进制的内容
 * @param base 进制单位（2，3，4...36）
 * @return 无符号大整数
 */
WMKC_PUBLIC(wmkcSize) wmkcMath_conversion WMKC_OPEN_API
WMKC_OF((wmkcCSTR content, wmkcByte base))
{
    wmkcChar *sequence = wmkcNull;
    wmkcSize size = strlen(content);
    wmkcSize res = 0;
    wmkcSize i;

    if(!wmkcMem_new(wmkcChar *, sequence, size)) {
        printf("wmkcMath_conversion: Failed to allocate memory for sequence.\n");
        return 0;
    }
    memcpy(sequence, content, size);

    if(base < 2 || base > 36) {
        printf("wmkcMath_conversion: base must be >= 2 and <= 36.\n");
        return 0;
    }

    for(i = 0; i < size; ++i) {
        if(!isalpha(sequence[i]) && !isalnum(sequence[i])) {
            printf("wmkcMath_conversion: Not a letter or number.\n");
            return 0;
        }
        if(sequence[i] >= 0x41 && sequence[i] <= 0x5a) {
            sequence[i] += 0x20;
        }
    }

    for(i = 0; i < size; ++i) {
        if(sequence[i] >= '0' && sequence[i] <= '9') {
            res += (sequence[i] - '0') * wmkcMath_pow(base, size-i-1);
        } else if(sequence[i] >= 'a' && sequence[i] <= 'z') {
            res += (sequence[i] - 'a' + 10) * wmkcMath_pow(base, size-i-1);
        } else {
            printf("Invalid contentber %c", sequence[i]);
            return 0;
        }
    }

    wmkcMem_free(sequence);
    return res;
}
