#include <utils/bytes.hh>

constexpr wuk::byte __ = 0x7f;

static constexpr wuk::byte hex_table[256] = {
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    0,   1,  2,  3,  4,  5,  6,  7,  8,  9, __, __, __, __, __, __,
    __, 10, 11, 12, 13, 14, 15, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, 10, 11, 12, 13, 14, 15, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __
};

static inline wuk::byte to_char(const wuk::byte &c)
{
    return (c + 0x57) - (-(c < 0xa) & 0x27);
}

std::vector<char>
wuk::utils::bytes_to_hex(const std::vector<wuk::byte> &buffer)
{
    if (buffer.empty()) {
        return {};
    }
    wuk::ulong old_length = buffer.size();
    wuk::ulong new_length = old_length * 2;
    std::vector<char> result(new_length);

    for (wuk::ulong i = 0; i < old_length; ++i) {
        result[i * 2]     = to_char(buffer[i] >> 4);
        result[i * 2 + 1] = to_char(buffer[i] & 0xf);
    }

    return result;
}

std::vector<wuk::byte>
wuk::utils::hex_to_bytes(const std::vector<char> &buffer)
{
    if (buffer.empty()) {
        return {};
    }
    if ((buffer.size() % 2) != 0) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::utils::hex_to_bytes",
            "Odd length is not allowed.");
    }
    wuk::ulong old_length = buffer.size();
    wuk::ulong new_length = old_length / 2;
    std::vector<wuk::byte> result(new_length);

    for (wuk::ulong i = 0; i < new_length; ++i) {
        wuk::byte top = hex_table[static_cast<wuk::byte>(buffer[i * 2])];
        wuk::byte bot = hex_table[static_cast<wuk::byte>(buffer[i * 2 + 1])];
        if ((top == __) || (bot == __)) {
            std::stringstream ss;
            ss  << "The " << (i * 2) << " character "
                << "is an invalid hex character.";
            throw wuk::Exception(wuk::Error::ERR, "wuk::utils::hex_to_bytes", ss.str());
        }
        result[i] = (top << 4) | bot;
    }

    return result;
}
