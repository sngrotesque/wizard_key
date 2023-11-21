#include <wmkc_binascii.hpp>

static const wmkcByte hexTable[256] = {
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    0,   1,  2,  3,  4,  5,  6,  7,  8,  9, 31, 31, 31, 31, 31, 31,
    31, 10, 11, 12, 13, 14, 15, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 10, 11, 12, 13, 14, 15, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31
};

wmkcByte wmkcBinascii::toTop(wmkcByte c)
{
    if((c >> 4) < 0x0a) {
        return (c >> 4) + 0x30;
    } else {
        return (c >> 4) + 0x57;
    }
}

wmkcByte wmkcBinascii::toBot(wmkcByte c)
{
    if((c & 0x0f) < 0x0a) {
        return (c & 0x0f) + 0x30;
    } else {
        return (c & 0x0f) + 0x57;
    }
}

std::string wmkcBinascii::b2a_hex(std::string content)
{
    if(content.empty()) {
        return std::string();
    }
    wmkcFast wmkcSize i;
    wmkcByte *src = (wmkcByte *)content.c_str();
    wmkcSize srcSize = content.size();

    wmkcByte *dst = new wmkcByte[srcSize << 1];
    if(!dst) {
        wmkcErr_exception(wmkcErr_ErrMemory, "wmkcBinascii::b2a_hex",
            "Failed to allocate memory for dst.");
    }

    for(i = 0; i < srcSize; ++i) {
        dst[i << 1]       = this->toTop(src[i]);
        dst[(i << 1) + 1] = this->toBot(src[i]);
    }

    std::string result((wmkcChar *)dst, srcSize << 1);
    delete[] dst;
    return result;
}

std::string wmkcBinascii::a2b_hex(std::string content)
{
    if(content.empty()) {
        return std::string();
    }
    wmkcSize srcIndex, dstIndex;
    wmkc_s32 top, bot;

    wmkcByte *src = (wmkcByte *)content.c_str();
    wmkcSize srcSize = content.size();
    wmkcByte *dst = wmkcNull;

    if(srcSize % 2) {
        wmkcErr_exception(wmkcErr_ErrType, "wmkcBinascii::a2b_hex",
            "Wrong type, should not be an odd length.");
    }

    if(!(dst = new wmkcByte[srcSize >> 1])) {
        wmkcErr_exception(wmkcErr_ErrMemory, "wmkcBinascii::a2b_hex",
            "Failed to allocate memory for dst.");
    }

    for(srcIndex = dstIndex = 0; srcIndex < srcSize; srcIndex += 2, ++dstIndex) {
        top = hexTable[src[srcIndex]];
        bot = hexTable[src[srcIndex + 1]];
        if((top | bot) > 30) {
            wmkcErr_exception(wmkcErr_ErrType, "wmkcBinascii_a2b_hex",
                "Wrong type, characters must be from 0 to f.");
        }
        dst[dstIndex] = (top << 4) + bot;
    }

    std::string result((wmkcChar *)dst, srcSize >> 1);
    delete[] dst;
    return result;
}
