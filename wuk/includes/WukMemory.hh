#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#ifdef WUK_PLATFORM_WINOS
#   include <Windows.h>
#endif
#include <malloc.h>
#include <memory>
#include <new>

static inline void forced_zeroing(volatile void *p, wuk::ulong length) noexcept
{
    volatile char *ptr = (volatile char *)p;
    do {
        *ptr++ = 0;
    } while (--length);
}

namespace wuk {
    inline LIBWUK_API void memory_secure(void *buffer, wuk::ulong length) noexcept
    {
#       if defined(WUK_PLATFORM_WINOS)
        SecureZeroMemory(buffer, length);
#       elif defined(WUK_PLATFORM_LINUX)
#       ifdef WUK_PLATFORM_ANDROID
        forced_zeroing(buffer, length);
#       else
        explicit_bzero(buffer, length);
#       endif
#       endif
    }

    inline LIBWUK_API void memory_zero(void *buffer, wuk::ulong length) noexcept
    {
        memset(buffer, 0x00, length);
    }

    template <typename T>
    inline LIBWUK_API T m_alloc(wuk::ulong length) noexcept
    {
        return static_cast<T>(malloc(length));
    }

    template <typename T>
    inline LIBWUK_API T m_realloc(T src, wuk::ulong length) noexcept
    {
        return static_cast<T>(realloc(src, length));
    }

    template <typename T>
    inline LIBWUK_API T *m_calloc(wuk::ulong length) noexcept
    {
        return static_cast<T *>(calloc(sizeof(T), length));
    }

    inline LIBWUK_API void m_free(void *p) noexcept
    {
        free(p);
    }
}

#endif /* WUK_SUPPORT */
