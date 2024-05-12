#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_MEMORY
#define WMKC_CPP_MEMORY

#ifdef WMKC_PLATFORM_WINOS
#   include <Windows.h>
#endif

namespace wmkc {
    wVoid memory_zero(wVoid *p, wSize n);
    wVoid memory_secure(wVoid *p, wSize n);
}

#endif /* WMKC_CPP_MEMORY */
#endif /* WMKC_SUPPORT */
