#include <WukBinascii.hh>

#include <utils/bytes.hh>

namespace wuk::binascii {
    wuk::Buffer b2a_hex(const wuk::Buffer &buffer) noexcept
    {
        const wuk::byte *p = buffer.data();
        wuk::ulong       n = buffer.size();

        std::vector<wuk::byte> input(p, p + n);
        std::vector<wuk::byte> output = wuk::utils::bytes_to_hex(input);

        wuk::Buffer result(output.data(), output.size());

        return result;
    }

    wuk::Buffer a2b_hex(const wuk::Buffer &buffer)
    {
        const wuk::byte *p = buffer.data();
        wuk::ulong       n = buffer.size();

        std::vector<wuk::byte> input(p, p + n);
        std::vector<wuk::byte> output = wuk::utils::hex_to_bytes(input);

        wuk::Buffer result(output.data(), output.size());

        return result;
    }
}
