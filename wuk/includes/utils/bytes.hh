#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukException.hh>
#include <core/WukEndianness.hh>
#include <vector>

namespace wuk::utils {
    LIBWUK_API std::vector<char> bytes_to_hex(const std::vector<wuk::byte> &buffer);
    LIBWUK_API std::vector<wuk::byte> hex_to_bytes(const std::vector<char> &buffer);

    template <typename T, bool reverse_endian = false>
    inline LIBWUK_API void pack_bytes(wuk::byte *buffer, wuk::u32 length, const T &val)
    {
        static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>, 
            "wuk::utils::pack_bytes requires numeric type");

        if (!buffer) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::utils::pack_bytes",
                "buffer is nullptr.");
        }
        if (length < sizeof(T)) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::utils::pack_bytes",
                "The buffer is too small, please adjust the size.");
        }
        const T tmp = (reverse_endian) ? swap_endian(val) : val;
        memcpy(buffer, &tmp, sizeof(T));
    }
}

#endif
