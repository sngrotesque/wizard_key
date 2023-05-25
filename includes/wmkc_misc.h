#ifndef WMKC_MISC
#define WMKC_MISC
#include <wmkc_conf.h>

WMKC_PUBLIC(wmkcVoid) wmkcMisc_PRINT WMKC_OPEN_API
WMKC_OF((wmkcByte *data, wmkcSize len, wmkcSize num, wmkcBool newline, wmkcBool tableChar));

WMKC_PUBLIC(wmkcVoid) wmkcMisc_PRINT_N WMKC_OPEN_API
WMKC_OF((wmkc_s32 *arr, wmkcSize size, wmkcBool newline));

WMKC_PUBLIC(wmkcVoid) wmkcMisc_PRINT_BOX WMKC_OPEN_API
WMKC_OF((wmkcByte *box, wmkcSize size, wmkcSize num, wmkcBool newline));

WMKC_PUBLIC(wmkcVoid) wmkcMisc_PRINT_RAW WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize size, wmkcBool newline));

#endif
