#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifdef WUK_PLATFORM_WINOS
#   include <Windows.h>
#endif
#include <malloc.h>

static inline void forced_zeroing(volatile void *p, wSize length)
{
    volatile char *ptr = (volatile char *)p;
    do {
        *ptr++ = 0;
    } while (--length);
}

namespace wuk {
    LIBWUK_API inline void memory_zero(void *buffer, wSize length)
    {
        memset(buffer, 0x00, length);
    }

    LIBWUK_API inline void memory_secure(void *buffer, wSize length)
    {
#       if defined(WUK_PLATFORM_WINOS)
        SecureZeroMemory(buffer, length);
#       elif defined(WUK_PLATFORM_LINUX)
#           ifdef WUK_PLATFORM_ANDROID
        forced_zeroing(buffer, length);
#           else
        explicit_bzero(buffer, length);
#           endif
#       endif
    }

    template <typename T>
    LIBWUK_API inline T m_alloc(wSize length) noexcept
    {
        return static_cast<T>(malloc(length));
    }

    template <typename T>
    LIBWUK_API inline T m_realloc(T src, wSize length) noexcept
    {
        return static_cast<T>(realloc(src, length));
    }

    template <typename T>
    LIBWUK_API inline T m_calloc(wSize length) noexcept
    {
        return static_cast<T>(calloc(sizeof(T), length));
    }

    LIBWUK_API inline void m_free(void *p) noexcept
    {
        free(p);
    }
}

#endif /* WUK_SUPPORT */
