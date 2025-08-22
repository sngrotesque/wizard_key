#include <WukBinascii.hh>

#include <utils/bytes.hh>

namespace wuk::binascii {
    std::string b2a_hex(const std::string &buffer)
    {
        const wuk::byte *p = \
            reinterpret_cast<const wuk::byte *>(buffer.data());
        wuk::ulong n = buffer.length();

        std::vector<wuk::byte> input(p, p + n);
        std::vector<char> output = wuk::utils::bytes_to_hex(input);

        std::string result(output.data(), output.size());

        return result;
    }

    std::string a2b_hex(const std::string &buffer)
    {
        const char *p = buffer.c_str();
        wuk::ulong  n = buffer.length();

        std::vector<char> input(p, p + n);
        std::vector<wuk::byte> output = wuk::utils::hex_to_bytes(input);

        std::string result(reinterpret_cast<const char *>(output.data()),
                        output.size());

        return result;
    }

    wuk::Buffer b2a_hex(const wuk::Buffer &buffer)
    {
        const wuk::byte *p = buffer.get_data();
        wuk::ulong       n = buffer.get_length();

        std::vector<wuk::byte> input(p, p + n);
        std::vector<char> output = wuk::utils::bytes_to_hex(input);

        wuk::Buffer result(reinterpret_cast<const wuk::byte *>(output.data()),
                        output.size());

        return result;
    }

    wuk::Buffer a2b_hex(const wuk::Buffer &buffer)
    {
        const char *p = buffer.get_cstr();
        wuk::ulong n = buffer.get_length();

        std::vector<char> input(p, p + n);
        std::vector<wuk::byte> output = wuk::utils::hex_to_bytes(input);

        wuk::Buffer result(output.data(), output.size());

        return result;
    }
}
