#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukException.hh>
#include <WukBuffer.hh>

namespace wuk::binascii {
    LIBWUK_API std::string b2a_hex(const std::string &buffer) noexcept;
    LIBWUK_API std::string a2b_hex(const std::string &buffer);

    LIBWUK_API wuk::Buffer b2a_hex(const wuk::Buffer &buffer) noexcept;
    LIBWUK_API wuk::Buffer a2b_hex(const wuk::Buffer &buffer);
}
#endif /* WUK_SUPPORT */
