#ifndef __SN_MATH__
#define __SN_MATH__

#ifdef __cplusplus
extern "C" {
#endif

#include <snConf.h>
#include <math.h>

#ifndef __SN_MEMORY
#include <stdlib.h>
#endif

// 求n的m次方
WMKC_PUBLIC(wmkcSize) snMath_Pow WMKC_OPEN_API
WMKC_OF((wmkcSize n, wmkcSize m));

// 求一个范围内的质数
WMKC_PUBLIC(wmkcSize *) snMath_PrimeNumber WMKC_OPEN_API
WMKC_OF((wmkcSize minValue, wmkcSize maxValue));

// 求n1和n2的公因数
WMKC_PUBLIC(wmkcSize *) snMath_CommonFactor WMKC_OPEN_API
WMKC_OF((wmkcSize n1, wmkcSize n2));

// 求_num的所有因数
WMKC_PUBLIC(wmkcSize *) snMath_Factor WMKC_OPEN_API
WMKC_OF((wmkcSize _num));

#ifdef __cplusplus
}
#endif

#endif // #ifndef __SN_MATH__
