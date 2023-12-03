#include <wmkc_base64.hpp>

// Base64编码表
static const wmkcByte _B64ET[64] = {
    65,   66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77, 78,  79,   80,
    81,   82,  83,  84,  85,  86,  87,  88,  89,  90,  97,  98,  99, 100, 101, 102,
    103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118,
    119, 120, 121, 122,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  43,  47};

// Base64解码表
static const wmkcByte _B64DT[123] = {
    0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,   0,  0,  62, 0,  0,  0, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,  0,  0,
    0,   0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,  0,  0,  0,  0,  0, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
    49, 50, 51};

wmkcSize wmkcBase64::encode_size(wmkcSize size)
{
    return (size % 3) ? ((size / 3 + 1) * 4) : (size / 3 * 4);
}

wmkcSize wmkcBase64::decode_size(std::string content)
{
    wmkcCSTR data = content.c_str();
    wmkcSize data_length = content.size();
    wmkcSize tmp_decode_size = data_length / 4 * 3;

    if(data[data_length - 1] == BASE64_PAD) --tmp_decode_size;
    if(data[data_length - 2] == BASE64_PAD) --tmp_decode_size;

    return tmp_decode_size;
}

std::string wmkcBase64::encode(std::string content)
{
    if(content.empty()) {
        return std::string();
    }
    wmkcSize dstIndex, srcIndex;
    wmkcSize srcSize = content.size();
    wmkcSize dstSize = this->encode_size(srcSize);

    wmkcByte *src = (wmkcByte *)content.c_str();
    wmkcByte *dst = new wmkcByte[dstSize];
    if(!dst) {
        wmkcErr_exception(wmkcErr_ErrMemory, "wmkcBase64::encode", "Failed to allocate memory for dst.");
    }

    for(dstIndex = srcIndex = 0; dstIndex < (dstSize - 2); srcIndex += 3, dstIndex += 4) {
        dst[dstIndex]   = _B64ET[src[srcIndex]           >> 2];
        dst[dstIndex+1] = _B64ET[(src[srcIndex]   & 0x3) << 4 | (src[srcIndex+1] >> 4)];
        dst[dstIndex+2] = _B64ET[(src[srcIndex+1] & 0xf) << 2 | (src[srcIndex+2] >> 6)];
        dst[dstIndex+3] = _B64ET[src[srcIndex+2]  & 0x3f];
    }

    switch(srcSize % 3) {
        case 1: dst[dstIndex - 2] = BASE64_PAD;
        case 2: dst[dstIndex - 1] = BASE64_PAD;
    }

    std::string result((wmkcChar *)dst, dstSize);
    delete[] dst;
    return result;
}

std::string wmkcBase64::decode(std::string content)
{
    if(content.empty()) {
        return std::string();
    }
    wmkcSize dstIndex, srcIndex;
    wmkcSize srcSize = content.size();
    if(srcSize & 3) {
        wmkcErr_exception(wmkcErr_ErrType, "wmkcBase64::decode", "The length of the src is incorrect.");
    }
    wmkcSize dstSize = this->decode_size(content);

    wmkcByte *src = (wmkcByte *)content.c_str();
    wmkcByte *dst = new wmkcByte[dstSize];
    if(!dst) {
        wmkcErr_exception(wmkcErr_ErrMemory, "wmkcBase64::decode", "Failed to allocate memory for dst.");
    }

    for(dstIndex = srcIndex = 0; srcIndex < (srcSize - 2); dstIndex += 3, srcIndex += 4) {
        dst[dstIndex]   = (_B64DT[src[srcIndex]]   << 2) |  (_B64DT[src[srcIndex+1]]  >> 4);
        dst[dstIndex+1] = (_B64DT[src[srcIndex+1]] << 4) | ((_B64DT[src[srcIndex+2]]) >> 2);
        dst[dstIndex+2] = (_B64DT[src[srcIndex+2]] << 6) |   _B64DT[src[srcIndex+3]];
    }

    std::string result((wmkcChar *)dst, dstSize);
    delete[] dst;
    return result;
}
