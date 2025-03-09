#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <filesystem>
#include <fstream>

namespace wuk {
    LIBWUK_API void hexdump(std::filesystem::path file_path);
    LIBWUK_API void hexdump(wByte *data, wSize length);
}

#endif /* WUK_SUPPORT */
