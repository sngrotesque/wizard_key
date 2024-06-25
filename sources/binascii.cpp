#include <binascii.hpp>

static const wByte hexTable[256] = {
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
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31};

wByte wmkc::Binascii::to_top(wByte c)
{
    return ((c >> 4) < 0xa) ? ((c >> 4) + 0x30) : ((c >> 4) + 0x57);
}

wByte wmkc::Binascii::to_bot(wByte c)
{
    return ((c & 0xf) < 0xa) ? ((c & 0xf) + 0x30) : ((c & 0xf) + 0x57);
}

std::string wmkc::Binascii::b2a_hex(std::string content)
{
    if(content.empty()) {
        return std::string();
    }
    wSize i;
    wByte *src = (wByte *)content.c_str();
    wSize srcSize = content.size();

    wByte *dst = new (std::nothrow) wByte[srcSize << 1];
    if(!dst) {
        throw wmkc::Exception(wmkcErr_ErrMemory, "wmkc::Binascii::b2a_hex",
            "Failed to allocate memory for dst.");
    }

    for(i = 0; i < srcSize; ++i) {
        dst[i << 1]       = this->to_top(src[i]);
        dst[(i << 1) + 1] = this->to_bot(src[i]);
    }

    std::string result((char *)dst, srcSize << 1);
    delete[] dst;
    return result;
}

std::string wmkc::Binascii::a2b_hex(std::string content)
{
    if(content.empty()) {
        return std::string();
    }
    wSize srcIndex, dstIndex;
    wS32 top, bot;

    wByte *src = (wByte *)content.c_str();
    wSize srcSize = content.size();
    wByte *dst = nullptr;

    if(srcSize & 1) {
        throw wmkc::Exception(wmkcErr_Err, "wmkc::Binascii::a2b_hex",
            "Wrong type, should not be an odd length.");
    }

    if(!(dst = new (std::nothrow) wByte[srcSize >> 1])) {
        throw wmkc::Exception(wmkcErr_ErrMemory, "wmkc::Binascii::a2b_hex",
            "Failed to allocate memory for dst.");
    }

    for(srcIndex = dstIndex = 0; srcIndex < srcSize; srcIndex += 2, ++dstIndex) {
        top = hexTable[src[srcIndex]];
        bot = hexTable[src[srcIndex + 1]];
        if((top == 31) || (bot == 31)) {
            delete[] dst;
            throw wmkc::Exception(wmkcErr_Err, "wmkc::binascii_a2b_hex",
                "Wrong type, characters must be from 0 to f.");
        }
        dst[dstIndex] = (top << 4) + bot;
    }

    std::string result((char *)dst, srcSize >> 1);
    delete[] dst;
    return result;
}
