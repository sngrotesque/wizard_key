#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukException.hh>
#include <core/WukEndianness.hh>
#include <vector>

namespace wuk::utils {
    LIBWUK_API std::vector<char> bytes_to_hex(const std::vector<wuk::byte> &buffer) noexcept;
    LIBWUK_API std::vector<wuk::byte> hex_to_bytes(const std::vector<char> &buffer);

    // 这两个函数的调用者应始终保证buffer所使用的内存大小大于等于T类型所占用的大小，否则极可能出现缓冲区溢出的安全漏洞。
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

    template <typename T, bool reverse_endian = false>
    inline LIBWUK_API T unpack_bytes(wuk::byte *buffer, wuk::u32 length)
    {
        static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>, 
            "wuk::utils::unpack_bytes requires numeric type");

        if (!buffer) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::utils::unpack_bytes",
                "buffer is nullptr.");
        }
        if (length < sizeof(T)) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::utils::pack_bytes",
                "The buffer is too small, please adjust the size.");
        }
        T tmp {0};
        memcpy(&tmp, buffer, sizeof(T));
        return (reverse_endian) ? swap_endian(tmp) : tmp;
    }
}

#endif
