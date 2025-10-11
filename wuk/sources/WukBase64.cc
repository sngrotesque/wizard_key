#include <WukBase64.hh>
#include <vector>

constexpr char base64pad = '=';
constexpr wuk::byte __ = 0x7f;

constexpr char b64en_table[65] = {
    "ABCDEFGHIJKLMNOP"
    "QRSTUVWXYZabcdef"
    "ghijklmnopqrstuv"
    "wxyz0123456789+/"
};

constexpr wuk::byte b64de_table[256] = {
    __, __, __, __,  __, __, __, __,  __, __, __, __,  __, __, __, __,
    __, __, __, __,  __, __, __, __,  __, __, __, __,  __, __, __, __,
    __, __, __, __,  __, __, __, __,  __, __, __, 62,  __, __, __, 63,
    52, 53, 54, 55,  56, 57, 58, 59,  60, 61, __, __,  __,  0, __, __,

    __,  0,  1,  2,   3,  4,  5,  6,   7,  8,  9, 10,  11, 12, 13, 14,
    15, 16, 17, 18,  19, 20, 21, 22,  23, 24, 25, __,  __, __, __, __,
    __, 26, 27, 28,  29, 30, 31, 32,  33, 34, 35, 36,  37, 38, 39, 40,
    41, 42, 43, 44,  45, 46, 47, 48,  49, 50, 51, __,  __, __, __, __,

    __, __, __, __,  __, __, __, __,  __, __, __, __,  __, __, __, __,
    __, __, __, __,  __, __, __, __,  __, __, __, __,  __, __, __, __,
    __, __, __, __,  __, __, __, __,  __, __, __, __,  __, __, __, __,
    __, __, __, __,  __, __, __, __,  __, __, __, __,  __, __, __, __,

    __, __, __, __,  __, __, __, __,  __, __, __, __,  __, __, __, __,
    __, __, __, __,  __, __, __, __,  __, __, __, __,  __, __, __, __,
    __, __, __, __,  __, __, __, __,  __, __, __, __,  __, __, __, __,
    __, __, __, __,  __, __, __, __,  __, __, __, __,  __, __, __, __
};

std::vector<wuk::byte> base64_encode(const std::vector<wuk::byte> &buffer) noexcept
{
    if (buffer.empty()) {
        return {};
    }
    wuk::ulong input_length = buffer.size();
    wuk::ulong output_length = (buffer.size() + 2) / 3 * 4;
    std::vector<wuk::byte> result(output_length);

    wuk::ulong i = 0, j = 0;
    for(; (i + 2) < input_length; i += 3, j += 4) {
        wuk::u32 v  =   (buffer[i]     << 16) |
                        (buffer[i+1]   << 8)  |
                        (buffer[i+2]);
        result[j]   =   b64en_table[(v >> 18) & 0x3f];
        result[j+1] =   b64en_table[(v >> 12) & 0x3f];
        result[j+2] =   b64en_table[(v >> 6 ) & 0x3f];
        result[j+3] =   b64en_table[(v      ) & 0x3f];
    }

    if (i < input_length) {
        wuk::u32 v  = buffer[i] << 16;
        result[j]   = b64en_table[(v >> 18) & 0x3f];
        result[j+1] = b64en_table[(v >> 12) & 0x3f];
        result[j+2] = (i+1 < input_length) \
                    ? b64en_table[((buffer[i+1] << 8) >> 6) & 0x3f] \
                    : base64pad;
        result[j+3] = base64pad;
    }

    return result;
}

std::vector<wuk::byte> base64_decode(const std::vector<wuk::byte> &buffer, bool strict)
{
    if (buffer.empty()) {
        return {};
    }
    const wuk::byte *input_data = buffer.data();
    const wuk::ulong input_length = buffer.size();
    bool padding_started = false;

    wuk::ulong             result_length = (input_length + 3) / 4 * 3;
    std::vector<wuk::byte> result_data(result_length);

    wuk::byte *bin_data       = result_data.data();
    wuk::byte *bin_data_start = bin_data;

    wuk::byte leftchar = 0; // 存储上一次迭代中剩余的字符位
    wuk::u32  quad_pos = 0; // 跟踪当前处理到编码块的哪个位置（0-3）
    wuk::u32  pads     = 0; // 记录填充字符的数量
    wuk::byte this_char;

    if (strict && (input_data[0] == base64pad)) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::base64::decode",
            "Leading padding not allowed.");
    }

    for (wuk::ulong i = 0; i < input_length; ++i) {
        this_char = input_data[i];

        if (this_char == base64pad) {
            padding_started = true;

            if (strict && !quad_pos) {
                throw wuk::Exception(wuk::Error::ERR, "wuk::base64::decode",
                    "Excess padding not allowed.");
            }

            if ((quad_pos > 1) && (quad_pos + (++pads) > 3)) {
                // 填充序列意味着不应该解析更多输入。
                // 在严格模式下如果填充符后有多余数据，将引发错误。
                if (strict && ((i + 1) < input_length)) {
                    throw wuk::Exception(wuk::Error::ERR, "wuk::base64::decode",
                        "Excess data after padding.");
                }

                goto done;
            }

            continue;
        }

        this_char = b64de_table[this_char];
        if (this_char == __) {
            if (strict) {
                throw wuk::Exception(wuk::Error::ERR, "wuk::base64::decode",
                    "Only base64 data is allowed.");
            }
            continue;
        }

        if (strict && padding_started) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::base64::decode",
                "Discontinuous padding not allowed.");
        }

        pads = 0;

        switch (quad_pos) {
        case 0:
            quad_pos = 1;
            leftchar = this_char;
            break;
        case 1:
            quad_pos = 2;
            *bin_data++ = (leftchar << 2) | (this_char >> 4);
            leftchar = this_char & 0xf;
            break;
        case 2:
            quad_pos = 3;
            *bin_data++ = (leftchar << 4) | (this_char >> 2);
            leftchar = this_char & 0x3;
            break;
        case 3:
            quad_pos = 0;
            *bin_data++ = (leftchar << 6) | (this_char);
            leftchar = 0;
            break;
        }
    }

    if (quad_pos) {
        std::string err_message;
        if (quad_pos == 1) {
            err_message = fmt::format(
                "Invalid base64-encoded string: "
                "number of data characters ({0}) "
                "cannot be 1 more than a multiple of 4.",
                ((bin_data - bin_data_start) / 3 * 4 + 1)
            );
        } else {
            err_message = "Incorrect padding.";
        }

        throw wuk::Exception(wuk::Error::ERR, "wuk::base64::decode",
            err_message);
    }

done:
    result_length = bin_data - bin_data_start;

    result_data.resize(result_length);

    return result_data;
}

namespace wuk::base64 {
    wuk::Buffer encode(const wuk::Buffer &buffer) noexcept
    {
        const wuk::byte *p = buffer.data();
        wuk::ulong       n = buffer.size();

        std::vector<wuk::byte> input(p, p + n);
        std::vector<wuk::byte> output = base64_encode(input);

        wuk::Buffer result(output.data(), output.size());

        return result;
    }

    wuk::Buffer decode(const wuk::Buffer &buffer, bool strict)
    {
        const char *p = buffer.c_str();
        wuk::ulong n = buffer.size();

        std::vector<wuk::byte> input(p, p + n);
        std::vector<wuk::byte> output = base64_decode(input, strict);

        wuk::Buffer result(output.data(), output.size());

        return result;
    }
}
