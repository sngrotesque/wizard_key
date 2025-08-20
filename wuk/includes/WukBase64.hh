#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <WukBuffer.hh>

namespace wuk::base64 {
    std::string encode(const std::string &buffer);
    std::string decode(const std::string &buffer, bool strict = false);

    wuk::Buffer encode(const wuk::Buffer &buffer);
    wuk::Buffer decode(const wuk::Buffer &buffer, bool strict = false);
}

#endif
