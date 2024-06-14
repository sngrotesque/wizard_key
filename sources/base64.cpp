#include <base64.hpp>

// Base64编码表
static const wByte _B64ET[64] = {
    65,   66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77, 78,  79,   80,
    81,   82,  83,  84,  85,  86,  87,  88,  89,  90,  97,  98,  99, 100, 101, 102,
    103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118,
    119, 120, 121, 122,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  43,  47};

// Base64解码表
static const wByte _B64DT[123] = {
    0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,   0,  0,  62, 0,  0,  0, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,  0,  0,
    0,   0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,  0,  0,  0,  0,  0, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
    49, 50, 51};

wSize wmkc::Base64::encode_size(wSize size)
{
    return (size % 3) ? ((size / 3 + 1) * 4) : (size / 3 * 4);
}

wSize wmkc::Base64::decode_size(std::string content)
{
    const char *data = content.c_str();
    wSize data_length = content.size();
    wSize tmp_decode_size = data_length / 4 * 3;

    if(data[data_length - 1] == BASE64_PAD) --tmp_decode_size;
    if(data[data_length - 2] == BASE64_PAD) --tmp_decode_size;

    return tmp_decode_size;
}

std::string wmkc::Base64::encode(std::string content)
{
    if(content.empty()) {
        return std::string();
    }
    wSize dst_i, src_i;
    wSize srcSize = content.size();
    wSize dstSize = this->encode_size(srcSize);

    wByte *src = (wByte *)content.c_str();
    wByte *dst = new wByte[dstSize];
    if(!dst) {
        throw wmkc::Exception(wmkcErr_ErrMemory, "wmkc::Base64::encode",
                                        "Failed to allocate memory for dst.");
    }

    for(dst_i = src_i = 0; dst_i < (dstSize - 2); src_i += 3, dst_i += 4) {
        dst[dst_i]   = _B64ET[src[src_i]           >> 2];
        dst[dst_i+1] = _B64ET[(src[src_i]   & 0x3) << 4 | (src[src_i+1] >> 4)];
        dst[dst_i+2] = _B64ET[(src[src_i+1] & 0xf) << 2 | (src[src_i+2] >> 6)];
        dst[dst_i+3] = _B64ET[src[src_i+2]  & 0x3f];
    }

    switch(srcSize % 3) {
        case 1: dst[dst_i - 2] = BASE64_PAD;
        case 2: dst[dst_i - 1] = BASE64_PAD;
    }

    std::string result((char *)dst, dstSize);
    delete[] dst;
    return result;
}

std::string wmkc::Base64::decode(std::string content)
{
    if(content.empty()) {
        return std::string();
    }
    wSize dst_i, src_i;
    wSize srcSize = content.size();
    if(srcSize & 3) {
        throw wmkc::Exception(wmkcErr_Err, "wmkc::Base64::decode",
                                        "The length of the src is incorrect.");
    }
    wSize dstSize = this->decode_size(content);

    wByte *src = (wByte *)content.c_str();
    wByte *dst = new wByte[dstSize];
    if(!dst) {
        throw wmkc::Exception(wmkcErr_ErrMemory, "wmkc::Base64::decode",
                                        "Failed to allocate memory for dst.");
    }

    for(dst_i = src_i = 0; src_i < (srcSize - 2); dst_i += 3, src_i += 4) {
        dst[dst_i]   = (_B64DT[src[src_i]]   << 2) |  (_B64DT[src[src_i+1]]  >> 4);
        dst[dst_i+1] = (_B64DT[src[src_i+1]] << 4) | ((_B64DT[src[src_i+2]]) >> 2);
        dst[dst_i+2] = (_B64DT[src[src_i+2]] << 6) |   _B64DT[src[src_i+3]];
    }

    std::string result((char *)dst, dstSize);
    delete[] dst;
    return result;
}
