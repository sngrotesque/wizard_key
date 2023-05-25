#ifndef WMKC_MATH
#define WMKC_MATH

#ifdef __cplusplus
extern "C" {
#endif

#include <wmkc_conf.h>
#include <math.h>

#ifndef WMKC_MEMORY
#include <stdlib.h>
#endif

// 求n的m次方
WMKC_PUBLIC(wmkcSize) wmkcMath_Pow WMKC_OPEN_API
WMKC_OF((wmkcSize n, wmkcSize m));

// 求一个范围内的质数
WMKC_PUBLIC(wmkcSize *) wmkcMath_PrimeNumber WMKC_OPEN_API
WMKC_OF((wmkcSize minValue, wmkcSize maxValue));

// 求n1和n2的公因数
WMKC_PUBLIC(wmkcSize *) wmkcMath_CommonFactor WMKC_OPEN_API
WMKC_OF((wmkcSize n1, wmkcSize n2));

// 求_num的所有因数
WMKC_PUBLIC(wmkcSize *) wmkcMath_Factor WMKC_OPEN_API
WMKC_OF((wmkcSize _num));

#ifdef __cplusplus
}
#endif

#endif
