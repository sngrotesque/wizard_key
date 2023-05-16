#include <snMath.h>

// 求n的m次方
SN_PUBLIC(snSize) snMath_Pow SN_OPEN_API
SN_FUNC_OF((snSize n, snSize m))
{
    snSize buf = n, i;
    for(i = 1; i < m; ++i)
        buf *= n;
    return buf;
}

// 求一个范围内的质数
SN_PUBLIC(snSize *) snMath_PrimeNumber SN_OPEN_API
SN_FUNC_OF((snSize minValue, snSize maxValue))
{
	register snSize x;
	register snSize i;
    register snSize count;
    snSize *p;

    if(minValue % 2 == 0 && minValue > 1)
        minValue += 1;

    if(minValue < 3) {
        p = (snSize *)malloc(2 * sizeof(snSize));
        *(p + 0) = 1;
        *(p + 1) = 2;
    }

    p = (snSize *)malloc(sizeof(snSize));
    count = 0;

	for(x = minValue; x <= maxValue; x += 2) {
		for(i = 2; i * i <= x; i++)
			if(x % i == 0)
				break;
		if(i * i > x) {
            *(p + count) = x;
            p = (snSize *)realloc(p, sizeof(snSize) * (count + 2));
            ++count;
        }
	}

    *(p + count) = 0;
    return p;
}

// 求n1和n2的公因数
SN_PUBLIC(snSize *) snMath_CommonFactor SN_OPEN_API
SN_FUNC_OF((snSize n1, snSize n2))
{
    static snSize total;
    static snSize n;
    static snSize count;
    static snSize *p;

    p = (snSize *)malloc(sizeof(snSize));

    if(n1 > n2)
        total = n2;
    else
        total = n1;

    for(count = 0, n = 2; n < total; ++n) {
        if(!((n1 % n) | (n2 % n))) {
            *(p + count) = n;
            p = (snSize *)realloc(p, sizeof(snSize) * (count + 2));
            ++count;
        }
    }

    *(p + count) = 0;
    return p;
}

// 求_num的所有因数
SN_PUBLIC(snSize *) snMath_Factor SN_OPEN_API
SN_FUNC_OF((snSize _num))
{
    static snSize n;
    static snSize count;
    static snSize *p;

    p = (snSize *)malloc(sizeof(snSize));

    for(count = 0, n = 2; n < _num; ++n) {
        if(!(_num % n)) {
            *(p + count) = n;
            p = (snSize *)realloc(p, (sizeof(snSize) * (count + 2)));
            ++count;
        }
    }

    *(p + count) = 0;
    return p;
}
