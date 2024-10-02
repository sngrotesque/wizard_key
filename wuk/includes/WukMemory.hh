#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_MEMORY
#define WUK_CPP_MEMORY

#ifdef WUK_PLATFORM_WINOS
#   include <Windows.h>
#endif

namespace wuk {
    LIBWUK_API void memory_zero(void *p, wSize n);
    LIBWUK_API void memory_secure(void *p, wSize n);
}

#endif /* WUK_CPP_MEMORY */
#endif /* WUK_SUPPORT */
