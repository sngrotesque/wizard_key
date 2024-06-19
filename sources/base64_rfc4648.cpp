#include <base64_rfc4648.hpp>

#define RFC4648_BASE64_PAD '='

constexpr char RFC4648_BASE64_ENCODE[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define BASE64_ENCODE_MAP_1(b, i) \
    RFC4648_BASE64_ENCODE[*(b+i) >> 2]
#define BASE64_ENCODE_MAP_2(b, i) \
    RFC4648_BASE64_ENCODE[((*(b+i) & 0x3) << 4) | (*(b+(i+1)) >> 4)]
#define BASE64_ENCODE_MAP_3(b, i) \
    RFC4648_BASE64_ENCODE[((*(b+(i+1)) & 0xf) << 2) | (*(b+(i+2)) >> 6)]
#define BASE64_ENCODE_MAP_4(b, i) \
    RFC4648_BASE64_ENCODE[*(b+(i+2)) & 0x3f]

wSize wmkc::Base64::get_encode_length(wSize length)
{
    return (length % 3) ? ((length / 3 + 1) * 4) : (length / 3 * 4);
}

char *wmkc::Base64::encode(const wByte *buffer, wSize &length)
{
    if(!buffer || !length) {
        throw wmkc::Exception(wmkcErr_ErrNULL,
            "wmkc::Base64::encode", "buffer is NULL.");
    }
    wSize result_len = this->get_encode_length(length);
    char *result = new(std::nothrow) char[result_len];
    if(!result) {
        throw wmkc::Exception(wmkcErr_ErrMemory,
            "wmkc::Base64::encode", "Failed to allocate memory for result.");
    }

    wSize src_index{0}, dst_index{0};
    for(; dst_index < (result_len - 2); src_index += 3, dst_index += 4) {
        result[dst_index]   = BASE64_ENCODE_MAP_1(buffer, src_index);
        result[dst_index+1] = BASE64_ENCODE_MAP_2(buffer, src_index);
        result[dst_index+2] = BASE64_ENCODE_MAP_3(buffer, src_index);
        result[dst_index+3] = BASE64_ENCODE_MAP_4(buffer, src_index);
    }

    switch(length % 3) {
        case 1: result[dst_index - 2] = RFC4648_BASE64_PAD;
        case 2: result[dst_index - 1] = RFC4648_BASE64_PAD;
    }

    length = result_len;
    return result;
}

std::string wmkc::Base64::encode(std::string buffer)
{
    if(buffer.empty()) {
        return std::string();
    }

    wByte *src_buffer = (wByte *)buffer.data();
    wSize length = buffer.size();

    char *dst_buffer = this->encode(src_buffer, length);

    std::string result{dst_buffer, length};
    delete[] dst_buffer;

    return result;
}

// This array comes from [Python](https://www.python.org/).
constexpr wByte RFC4648_BASE64_DECODE[256] = {
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,62, -1,-1,-1,63,
    52,53,54,55, 56,57,58,59, 60,61,-1,-1, -1, 0,-1,-1,
    -1, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
    15,16,17,18, 19,20,21,22, 23,24,25,-1, -1,-1,-1,-1,
    -1,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,
    41,42,43,44, 45,46,47,48, 49,50,51,-1, -1,-1,-1,-1,

    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1};

