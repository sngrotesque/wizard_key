#include <base64.hpp>

#define BASE64PAD '='

constexpr char b64en_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
constexpr wByte b64de_table[256] = {
    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
    255,255,255,255, 255,255,255,255, 255,255,255, 62, 255,255,255, 63,
    52 , 53, 54, 55,  56, 57, 58, 59,  60, 61,255,255, 255,  0,255,255,

    255,  0,  1,  2,   3,  4,  5,  6,   7,  8,  9, 10,  11, 12, 13, 14,
    15 , 16, 17, 18,  19, 20, 21, 22,  23, 24, 25,255, 255,255,255,255,
    255, 26, 27, 28,  29, 30, 31, 32,  33, 34, 35, 36,  37, 38, 39, 40,
    41 , 42, 43, 44,  45, 46, 47, 48,  49, 50, 51,255, 255,255,255,255,

    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,

    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255,
    255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255};

// Encoding, definition
#define BASE64_EN_MAP_1(b, i) \
    b64en_table[b[i] >> 2]
#define BASE64_EN_MAP_2(b, i) \
    b64en_table[((b[i] & 0x3) << 4) | (b[i+1] >> 4)]
#define BASE64_EN_MAP_3(b, i) \
    b64en_table[((b[i+1] & 0xf) << 2) | (b[i+2] >> 6)]
#define BASE64_EN_MAP_4(b, i) \
    b64en_table[b[i+2] & 0x3f]

wSize wmkc::Base64::get_encode_length(wSize length)
{
    return (length % 3) ? (length / 3 + 1) * 4 : (length / 3 * 4);
}

char *wmkc::Base64::encode(const wByte *buffer, wSize &length)
{
    if(!buffer || !length) {
        throw wmkc::Exception(wmkcErr_ErrNULL, "wmkc::Base64::encode",
            "buffer is NULL.");
    }
    wSize src_index{0}, dst_index{0};
    wSize result_length = this->get_encode_length(length);

    char *result = new (std::nothrow) char[result_length + 1];
    if(!result) {
        throw wmkc::Exception(wmkcErr_ErrMemory, "wmkc::Base64::encode",
            "Failed to allocate memory for result.");
    }

    for(; dst_index < result_length; src_index += 3, dst_index += 4) {
        result[dst_index]   = BASE64_EN_MAP_1(buffer, src_index);
        result[dst_index+1] = BASE64_EN_MAP_2(buffer, src_index);
        result[dst_index+2] = BASE64_EN_MAP_3(buffer, src_index);
        result[dst_index+3] = BASE64_EN_MAP_4(buffer, src_index);
    }

    // 此处的switch不需要break语句！
    switch(length % 3) {
        case 1:
            result[dst_index - 2] = BASE64PAD;
        case 2:
            result[dst_index - 1] = BASE64PAD;
    }

    result[result_length] = 0x00;
    length = result_length;

    return result;
}

std::string wmkc::Base64::encode(std::string _buffer)
{
    if(_buffer.empty()) {
        throw wmkc::Exception(wmkcErr_ErrNULL, "wmkc::Base64::encode",
            "buffer is NULL.");
    }

    wByte *buffer = (wByte *)_buffer.data();
    wSize length = _buffer.size();

    char *result = this->encode(buffer, length);

    std::string _result{result, length};
    delete[] result;

    return _result;
}

// Decoding, definition
wSize wmkc::Base64::get_decode_length(wSize length)
{
    return length / 4 * 3;
}

wByte *wmkc::Base64::decode(const char *buffer, wSize &length)
{
    if(!buffer || !length) {
        throw wmkc::Exception(wmkcErr_ErrNULL, "wmkc::Base64::decode",
            "buffer is NULL.");
    }
    if(length & 3) {
        throw wmkc::Exception(wmkcErr_Err, "wmkc::Base64::decode",
            "The length of encoded data must be a multiple of 4.");
    }

    wSize src_index{0}, dst_index{0};
    wSize result_length = this->get_decode_length(length);
    wByte this_ch;

    wByte *result = new (std::nothrow) wByte[result_length + 1];
    if(!result) {
        throw wmkc::Exception(wmkcErr_ErrMemory, "wmkc::Base64::encode",
            "Failed to allocate memory for result.");
    }

    for(; src_index < length; ++src_index) {
        /*  如果要跳过前置填充符，并正常处理余下的数据，那么
            请使用`continue`，否则使用`break`直接跳出循环。*/
        if(buffer[src_index] == BASE64PAD) {
            continue;
        }

        /*  此处的处理将直接越过非Base64编码表中的字符，正常
            处理下一个字符。 */
        this_ch = b64de_table[(wByte)buffer[src_index]];
        if(this_ch == 255) {
            continue;
        }

        switch(src_index & 3) {
            case 0:
                result[dst_index] = (this_ch << 2) & 0xff;
                break;
            case 1:
                result[dst_index++] |= (this_ch >> 4) & 0x3;
                result[dst_index] = (this_ch & 0xf) << 4;
                break;
            case 2:
                result[dst_index++] |= (this_ch >> 2) & 0xf;
                result[dst_index] = (this_ch & 3) << 6;
                break;
            case 3:
                result[dst_index++] |= this_ch;
                break;
        }
    }

    length = dst_index;
    return result;
}

std::string wmkc::Base64::decode(std::string _buffer)
{
    if(_buffer.empty()) {
        throw wmkc::Exception(wmkcErr_ErrNULL, "wmkc::Base64::decode",
            "buffer is NULL.");
    }

    const char *buffer = _buffer.c_str();
    wSize length = _buffer.size();

    wByte *result = this->decode(buffer, length);

    std::string _result{(char *)result, length};
    delete[] result;

    return _result;
}
