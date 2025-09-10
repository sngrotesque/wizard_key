#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukException.hh>
#include <WukBuffer.hh>

namespace wuk::base64 {
    LIBWUK_API std::string encode(const std::string &buffer) noexcept;
    LIBWUK_API std::string decode(const std::string &buffer, bool strict = false);

    LIBWUK_API wuk::Buffer encode(const wuk::Buffer &buffer) noexcept;
    LIBWUK_API wuk::Buffer decode(const wuk::Buffer &buffer, bool strict = false);
}

#endif
