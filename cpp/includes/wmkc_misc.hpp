#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_MISC
#define WMKC_CPP_MISC
#include <wmkc_color.hpp>

namespace wmkcMisc {
    wmkcVoid PRINT(const wmkcByte *data, wmkcSize len, wmkcSize num, wmkcBool newline, wmkcBool tableChar);
    wmkcVoid PRINT_N(const wmkcByte *arr, wmkcSize size, wmkcBool newline);
    wmkcVoid PRINT_BOX(const wmkcByte *box, wmkcSize size, wmkcSize num, wmkcBool newline);
    wmkcVoid PRINT_RAW(const wmkcByte *buf, wmkcSize size, wmkcBool newline);
}

#endif /* WMKC_CPP_MISC */
#endif /* WMKC_SUPPORT */
