#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_MEMORY
#define WMKC_CPP_MEMORY

#ifdef WMKC_PLATFORM_WINOS
#   include <Windows.h>
#endif

namespace wmkc {
    LIBWMKC_API void memory_zero(void *p, wSize n);
    LIBWMKC_API void memory_secure(void *p, wSize n);
}

#endif /* WMKC_CPP_MEMORY */
#endif /* WMKC_SUPPORT */
