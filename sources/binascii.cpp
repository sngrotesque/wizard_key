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

char *wmkc::Binascii::b2a_hex(const wByte *buffer, wSize &length)
{
    if(!buffer || !length) {
        throw wmkc::Exception(wmkcErr_ErrNULL, "wmkc::Binascii::b2a_hex",
            "buffer is NULL.");
    }

    char *result = new (std::nothrow) char[(length << 1) + 1];
    if(!result) {
        throw wmkc::Exception(wmkcErr_ErrMemory, "wmkc::Binascii::b2a_hex",
            "Failed to allocate memory for result.");
    }
    *(result + (length << 1)) = 0x00;

    for(wSize i = 0; i < length; ++i) {
        *(result + (i << 1))       = this->to_top(*(buffer + i));
        *(result + ((i << 1) + 1)) = this->to_bot(*(buffer + i));
    }

    length <<= 1;
    return result;
}

wByte *wmkc::Binascii::a2b_hex(const char *buffer, wSize &length)
{
    if(!buffer || !length) {
        throw wmkc::Exception(wmkcErr_ErrNULL, "wmkc::Binascii::a2b_hex",
            "buffer is NULL.");
    }
    if(length & 1) {
        throw wmkc::Exception(wmkcErr_Err, "wmkc::Binascii::a2b_hex",
            "Odd length is not allowed.");
    }
    wByte *result = new (std::nothrow) wByte[(length >> 1) + 1];
    if(!result) {
        throw wmkc::Exception(wmkcErr_ErrMemory, "wmkc::Binascii::a2b_hex",
            "Failed to allocate memory for result.");
    }
    wByte *buffer_p = (wByte *)buffer;
    *(result + (length >> 1)) = 0x00;

    wSize bi, ri;
    wS32 top, bot;
    for(bi = ri = 0; bi < length; bi += 2, ++ri) {
        top = hexTable[*(buffer_p + bi)];
        bot = hexTable[*(buffer_p + (bi + 1))];
        if((top == 31) || (bot == 31)) {
            delete[] result;
            throw wmkc::Exception(wmkcErr_Err, "wmkc::Binascii::a2b_hex",
                "characters must be from 0 to f.");
        }
        *(result + ri) = (top << 4) + bot;
    }

    length >>= 1;
    return result;
}

std::string wmkc::Binascii::b2a_hex(std::string _buffer)
{
    if(_buffer.empty()) {
        return std::string();
    }
    wByte *buffer = (wByte *)_buffer.data();
    wSize length = _buffer.size();
    char *result = nullptr;

    try {
        result = this->b2a_hex(buffer, length);
    } catch (std::exception &e) {
        throw;
    }

    std::string result_string{result, length};
    delete[] result;

    return result_string;
}

std::string wmkc::Binascii::a2b_hex(std::string _buffer)
{
    if(_buffer.empty()) {
        return std::string();
    }
    char *buffer = _buffer.data();
    wSize length = _buffer.size();
    wByte *result = nullptr;

    try {
        result = this->a2b_hex(buffer, length);
    } catch (std::exception &e) {
        throw;
    }

    std::string result_string{(char *)result, length};
    delete[] result;

    return result_string;
}
