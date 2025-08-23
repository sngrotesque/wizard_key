#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#if defined(WUK_PLATFORM_WINOS)
#   define WUK_NATIVE_LE // Windows is little-endian by default
#else
#   include <endian.h>
#   if defined(__BYTE_ORDER__) && \
       defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__)
#       if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#           define WUK_NATIVE_LE
#       elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#           define WUK_NATIVE_BE
#       endif
#   endif
#endif

namespace wuk {
    template <typename T>
    void reversal_array(T *buffer, wSize length)
    {
        for (wSize i = 0; i < (length / 2); ++i) {
            T tmp = buffer[i];
            buffer[i] = buffer[length - 1 - i];
            buffer[length - 1 - i] = tmp;
        }
    }
}

#endif // WUK_SUPPORT
