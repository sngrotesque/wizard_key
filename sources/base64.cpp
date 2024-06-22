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
        result[dst_index]   = b64en_table[buffer[src_index] >> 2];
        result[dst_index+1] = b64en_table[((buffer[src_index] & 0x3) << 4) | (buffer[src_index+1] >> 4)];
        result[dst_index+2] = b64en_table[((buffer[src_index+1] & 0xf) << 2) | (buffer[src_index+2] >> 6)];
        result[dst_index+3] = b64en_table[buffer[src_index+2] & 0x3f];
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
    if(*buffer == BASE64PAD) {
        throw wmkc::Exception(wmkcErr_Err, "wmkc::Base64::decode",
            "Leading padding not allowed.");
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
        /* 此项会默认执行：不允许前导填充+不允许简短填充 */
        if(buffer[src_index] == BASE64PAD) {
            break;
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

/*
* The original implementation of this function comes from
* lines 387 to 522 in [Python](https://www.python.org/downloads/release/python-3124/)
* code file `Modules/binascii.c`.
*/
wByte *wmkc::Base64::pyDecode(const char *buffer, wSize &length, wBool strict_mode)
{
    std::string error_message;

    const wByte *ascii_data = (const wByte *)buffer;
    wSize ascii_len = length;
    wBool padding_started = 0;

    wSize bin_len = this->get_decode_length(ascii_len);
    wByte *bin_data = new (std::nothrow) wByte[bin_len + 1];
    if(!bin_data) {
        throw wmkc::Exception(wmkcErr_ErrMemory, "wmkc::Base64::pyDecode",
            "Failed to allocate memory for bin_data.");
    }
    wByte *bin_data_start = bin_data;
    bin_data[bin_len] = 0x0;

    wByte leftchar = 0; // 定义一个变量来存储上一次迭代中剩余的字符位
    wU32 quad_pos = 0; // 定义一个变量来跟踪当前处理到Base64编码块中的哪个位置（0到3）
    wU32 pads = 0; // 定义一个变量来计数填充字符的数量

    if(strict_mode && (ascii_len > 0) && (*ascii_data == BASE64PAD)) {
        error_message = "Leading padding not allowed.";
        goto error_end;
    }

    wSize i;
    wByte this_ch;
    for(i = 0; i < ascii_len; ++i) {
        this_ch = ascii_data[i];

        if(this_ch == BASE64PAD) {
            padding_started = true;
            bin_len--;

            if(strict_mode && (!quad_pos)) {
                error_message = "Excess padding not allowed.";
                goto error_end;
            }

            if((quad_pos >= 2) && (quad_pos + (++pads) >= 4)) {

                if(strict_mode && ((i + 1) < ascii_len)) {
                    error_message = "Excess data after padding.";
                    goto error_end;
                }

                goto done;
            }

            continue;
        }

        this_ch = b64de_table[this_ch];
        if(this_ch == 255) {
            if(strict_mode) {
                error_message = "Only base64 data is allowed.";
                goto error_end;
            }
            continue;
        }

        if(strict_mode && padding_started) {
            error_message = "Discontinuous padding not allowed.";
            goto error_end;
        }

        pads = 0;

        switch(quad_pos) {
        case 0:
            quad_pos = 1;
            leftchar = this_ch;
            break;
        case 1:
            quad_pos = 2;
            *bin_data++ = (leftchar << 2) | (this_ch >> 4);
            leftchar = this_ch & 0xf;
            break;
        case 2:
            quad_pos = 3;
            *bin_data++ = (leftchar << 4) | (this_ch >> 2);
            leftchar = this_ch & 0x3;
            break;
        case 3:
            quad_pos = 0;
            *bin_data++ = (leftchar << 6) | (this_ch);
            leftchar = 0;
            break;
        }
    }

    if(quad_pos) {
        if(quad_pos == 1) {
            char tmpMsg[128]{};
            snprintf(tmpMsg, sizeof(tmpMsg),
                    "Invalid base64-encoded string: "
                    "number of data characters (%zd) cannot be 1 more "
                    "than a multiple of 4",
                    (bin_data - bin_data_start) / 3 * 4 + 1);
            error_message = tmpMsg;
            goto error_end;
        } else {
            error_message = "Incorrect padding.";
            goto error_end;
        }
        error_end:
        delete[] bin_data;
        throw wmkc::Exception(wmkcErr_Err, "wmkc::Base64::pyDecode", error_message);
    }

done:
    length = bin_len;
    return bin_data_start;
}
