#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifdef WUK_PLATFORM_WINOS
#   include <Windows.h>
#endif
#include <malloc.h>

namespace wuk {
    LIBWUK_API void memory_zero(void *p, wSize n)
    {
        memset(p, 0x00, n);
    }

    LIBWUK_API void memory_secure(void *p, wSize n)
    {
#       if defined(WUK_PLATFORM_WINOS)
        SecureZeroMemory(p, n);
#       elif defined(WUK_PLATFORM_LINUX)
        explicit_bzero(p, n);
#       endif
    }

    template <typename T>
    LIBWUK_API T m_alloc(wSize length) noexcept
    {
        return static_cast<T>(malloc(length));
    }

    template <typename T>
    LIBWUK_API T m_realloc(T src, wSize length) noexcept
    {
        return static_cast<T>(realloc(src, length));
    }

    template <typename T>
    LIBWUK_API T m_calloc(wSize length)
    {
        return static_cast<T>(calloc(sizeof(T), length));
    }

    LIBWUK_API void m_free(void *p) noexcept
    {
        free(p);
    }
}

#endif /* WUK_SUPPORT */
