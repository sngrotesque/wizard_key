#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <vector>

namespace wuk::utils {
    std::vector<char> bytes_to_hex(const std::vector<wuk::byte> &buffer);
    std::vector<wuk::byte> hex_to_bytes(const std::vector<char> &buffer);
}

#endif /* WUK_SUPPORT */
