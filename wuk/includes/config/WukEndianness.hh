#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#if defined(WUK_PLATFORM_WINOS)
#   define WUK_IS_LITTLE_ENDIAN() true
#else
#   include <endian.h>
#   if __BYTE_ORDER__
#       if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#           define WUK_IS_LITTLE_ENDIAN() true
#       elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#           define WUK_IS_LITTLE_ENDIAN() false
#       endif
#   else
constexpr bool WUK_IS_LITTLE_ENDIAN()
{
    wU32 n{1};
    return (*((char *)&n) == 1);
}
#   endif
#endif

namespace wuk {
    template <typename T>
    void reversal_array(T *buffer, wSize length)
    {
        for (wU32 i = 0; i < (length >> 1); ++i) {
            const T swap = buffer[i];
            buffer[i] = buffer[length - i - 1];
            buffer[length - i - 1] = swap;
        }
    }
}

#endif
