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
SN_PUBLIC(snSize) snMath_Pow SN_OPEN_API
SN_FUNC_OF((snSize n, snSize m));

// 求一个范围内的质数
SN_PUBLIC(snSize *) snMath_PrimeNumber SN_OPEN_API
SN_FUNC_OF((snSize minValue, snSize maxValue));

// 求n1和n2的公因数
SN_PUBLIC(snSize *) snMath_CommonFactor SN_OPEN_API
SN_FUNC_OF((snSize n1, snSize n2));

// 求_num的所有因数
SN_PUBLIC(snSize *) snMath_Factor SN_OPEN_API
SN_FUNC_OF((snSize _num));

#ifdef __cplusplus
}
#endif

#endif // #ifndef __SN_MATH__
