#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_MISC
#define WMKC_CPP_MISC
#include <wmkc_color.h>

namespace wmkcMisc {
    wmkcVoid PRINT(wmkcByte *data, wmkcSize len, wmkcSize num, wmkcBool newline, wmkcBool tableChar);
    wmkcVoid PRINT_N(wmkcByte *arr, wmkcSize size, wmkcBool newline);
    wmkcVoid PRINT_BOX(wmkcByte *box, wmkcSize size, wmkcSize num, wmkcBool newline);
    wmkcVoid PRINT_RAW(wmkcByte *buf, wmkcSize size, wmkcBool newline);
}

#endif /* WMKC_CPP_MISC */
#endif /* WMKC_SUPPORT */
