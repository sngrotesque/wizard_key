#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <type_traits>

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
    inline void reversal_array(T *buffer, wuk::ulong length)
    {
        for (wuk::ulong i = 0; i < (length / 2); ++i) {
            T tmp = buffer[i];
            buffer[i] = buffer[length - 1 - i];
            buffer[length - 1 - i] = tmp;
        }
    }

    template <typename T>
    inline T swap_endian(const T &val)
    {
        if (!std::is_integral_v<T> && !std::is_floating_point_v<T>) {
            return val;
        }
        if constexpr (sizeof(T) < 2) {
            return val;
        }
        constexpr wuk::u32 T_size = sizeof(T);
        wuk::byte buffer[T_size] {0};
        memcpy(buffer, &val, T_size);

        for (wuk::u32 i = 0; i < (T_size / 2); ++i) {
            const wuk::byte swap = buffer[i];
            buffer[i] = buffer[T_size - 1 - i];
            buffer[T_size - 1 - i] = swap;
        }

        T res {0};
        memcpy(&res, buffer, T_size);
        return res;
    }
}

#endif // WUK_SUPPORT
