#include <snMath.h>

// 求n的m次方
WMKC_PUBLIC(wmkcSize) snMath_Pow WMKC_OPEN_API
WMKC_OF((wmkcSize n, wmkcSize m))
{
    wmkcSize buf = n, i;
    for(i = 1; i < m; ++i)
        buf *= n;
    return buf;
}

// 求一个范围内的质数
WMKC_PUBLIC(wmkcSize *) snMath_PrimeNumber WMKC_OPEN_API
WMKC_OF((wmkcSize minValue, wmkcSize maxValue))
{
	wmkcFast wmkcSize x;
	wmkcFast wmkcSize i;
    wmkcFast wmkcSize count;
    wmkcSize *p;

    if(minValue % 2 == 0 && minValue > 1)
        minValue += 1;

    if(minValue < 3) {
        p = (wmkcSize *)malloc(2 * sizeof(wmkcSize));
        *(p + 0) = 1;
        *(p + 1) = 2;
    }

    p = (wmkcSize *)malloc(sizeof(wmkcSize));
    count = 0;

	for(x = minValue; x <= maxValue; x += 2) {
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

// 求n1和n2的公因数
WMKC_PUBLIC(wmkcSize *) snMath_CommonFactor WMKC_OPEN_API
WMKC_OF((wmkcSize n1, wmkcSize n2))
{
    static wmkcSize total;
    static wmkcSize n;
    static wmkcSize count;
    static wmkcSize *p;

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

// 求_num的所有因数
WMKC_PUBLIC(wmkcSize *) snMath_Factor WMKC_OPEN_API
WMKC_OF((wmkcSize _num))
{
    static wmkcSize n;
    static wmkcSize count;
    static wmkcSize *p;

    p = (wmkcSize *)malloc(sizeof(wmkcSize));

    for(count = 0, n = 2; n < _num; ++n) {
        if(!(_num % n)) {
            *(p + count) = n;
            p = (wmkcSize *)realloc(p, (sizeof(wmkcSize) * (count + 2)));
            ++count;
        }
    }

    *(p + count) = 0;
    return p;
}
