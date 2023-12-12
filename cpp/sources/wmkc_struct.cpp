#include <wmkc_struct.hpp>

static const wmkcByte wmkcStruct_byteOrder[3] = {
    '@', // 按原样顺序
    '>', // 按大端序
    '<'  // 按小端序
};

static const wmkcByte wmkcStruct_format_symbol[7] = {
    'B', // unsigned char  -> uint8_t
    'H', // unsigned short -> uint16_t
    'I', // unsigned int   -> uint32_t
    'Q', // unsigned long  -> uint64_t
    'N', // unsigned long long -> size_t
    'f', // float
    'd', // double
};

std::string wmkcStruct::pack(std::string format, ...)
{
    if(format.empty()) {
        return std::string();
    }

    const wmkcChar *format_content = format.c_str();
    const wmkcSize format_length = format.size();
    wmkcBool littleEndian;
    wmkcSize index;
    std::string result;

    const wmkcByte *p = std::find(wmkcStruct_byteOrder, wmkcStruct_byteOrder + sizeof(wmkcStruct_byteOrder), *format_content);
    
    // for(index = 0; index < format_length; ++index) {
    // }

    return result;
}

std::vector<wmkcSize> wmkcStruct::unpack(std::string format, ...)
{
    std::vector<wmkcSize> result;


    return result;
}

