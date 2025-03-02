#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <fstream>

namespace wuk {
    template <typename T>
    LIBWUK_API void hexdump(T file_path);
    LIBWUK_API void hexdump(wByte *data, wSize length);
}

#endif /* WUK_SUPPORT */
