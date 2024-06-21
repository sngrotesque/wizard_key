#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_MISC
#define WMKC_CPP_MISC
#include <config/color.hpp>

namespace wmkc {
    namespace misc {
        LIBWMKC_API void PRINT_HEX(const wByte *data, wSize len, wSize num, wBool newline,
                                wBool tableChar);
        LIBWMKC_API void PRINT_N(const wByte *arr, wSize size, wBool newline);
        LIBWMKC_API void PRINT_BOX(const wByte *box, wSize size, wSize num, wBool newline);
        LIBWMKC_API void PRINT_RAW(const wByte *buf, wSize size, wBool newline);
    }
}

#endif /* WMKC_CPP_MISC */
#endif /* WMKC_SUPPORT */
