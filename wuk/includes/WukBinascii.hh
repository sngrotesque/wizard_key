#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <WukBuffer.hh>

namespace wuk::binascii {
    std::string b2a_hex(const std::string &buffer);
    std::string a2b_hex(const std::string &buffer);

    wuk::Buffer b2a_hex(const wuk::Buffer &buffer);
    wuk::Buffer a2b_hex(const wuk::Buffer &buffer);
}

#endif /* WUK_SUPPORT */
