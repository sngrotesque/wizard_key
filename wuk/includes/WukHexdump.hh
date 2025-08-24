#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukException.hh>
#include <filesystem>

namespace fs = std::filesystem;

namespace wuk {
    LIBWUK_API void hexdump(fs::path file_path);
    LIBWUK_API void hexdump(wuk::byte *data, wuk::ulong length);
}

#endif /* WUK_SUPPORT */
