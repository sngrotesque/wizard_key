#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukEndianness.hh>
#include <WukBuffer.hh>

namespace wuk::utils {
    template <typename T, bool reverse_endianness = false>
    inline wuk::Buffer pack_bytes(T val)
    {
        static_assert(
            std::is_integral_v<T> || std::is_floating_point_v<T>,
            "wuk::utils::pack_bytes requires numeric type."
        );
        wuk::Buffer result{sizeof(T), 0};

        T swap = (!reverse_endianness) ? val : swap_endian(val);
        memcpy(result.write<wuk::byte>(sizeof(T)), &swap, sizeof(T));

        return result;
    }

    template <typename T, bool reverse_endianness = false>
    inline T unpack_bytes(const wuk::Buffer &buffer)
    {
        static_assert(
            std::is_integral_v<T> || std::is_floating_point_v<T>,
            "wuk::utils::unpack_bytes requires numeric type."
        );
        if (buffer.capacity() < sizeof(T)) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::utils::unpack_bytes",
                "The buffer is too small, please adjust the size.");
        }
        T swap{};
        memcpy(&swap, buffer.data(), sizeof(T));

        return (!reverse_endianness) ? swap : swap_endian(swap);
    }
}
#endif
