#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukException.hh>
#include <vector>

namespace wuk::utils {
    LIBWUK_API std::vector<wuk::byte> bytes_to_hex(const std::vector<wuk::byte> &buffer) noexcept;
    LIBWUK_API std::vector<wuk::byte> hex_to_bytes(const std::vector<wuk::byte> &buffer);
}

#endif
