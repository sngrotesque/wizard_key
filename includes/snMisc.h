#ifndef __SN_MISC__
#define __SN_MISC__
#include <snConf.h>

SN_PUBLIC(snVoid) snMisc_PRINT SN_OPEN_API
SN_FUNC_OF((snByte *data, snSize len, snSize num, snBool newline, snBool tableChar));

SN_PUBLIC(snVoid) snMisc_PRINT_N SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size, snBool newline));

SN_PUBLIC(snVoid) snMisc_PRINT_BOX SN_OPEN_API
SN_FUNC_OF((snByte *box, snSize size, snSize num, snBool newline));

SN_PUBLIC(snVoid) snMisc_PRINT_RAW SN_OPEN_API
SN_FUNC_OF((snByte *buf, snSize size, snBool newline));

#endif
