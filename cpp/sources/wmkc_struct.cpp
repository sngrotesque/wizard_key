#include <wmkc_struct.hpp>

static const wmkcByte byteOrder[4] = {
    '@', // 按原样顺序
    '>', // 按大端序
    '<', // 按小端序
    '!'  // 网络端序（大端序）
};

static const wmkcByte formatSymbol[8] = {
    'B', // unsigned char  -> uint8_t
    'H', // unsigned short -> uint16_t
    'I', // unsigned int   -> uint32_t
    'Q', // unsigned long  -> uint64_t
    'N', // unsigned long long -> size_t
    'f', // float
    'd', // double
    's' // bytes array
};

wmkcBool verifySymbol(const wmkcByte *p, const wmkc_u32 n, wmkcChar in)
{
    const wmkcByte *begin_ptr = p;
    const wmkcByte *end_ptr = p + n;
    return (std::find(begin_ptr, end_ptr, in) != end_ptr);
}

std::string wmkcStruct::pack(std::string format, std::vector<wmkcSize> args)
{
    if(format.empty()) {
        return std::string();
    }
    wmkcBool correctByteOrder = verifySymbol(byteOrder, sizeof(byteOrder), format[0]);
    wmkcBool correctFormatSymbol = verifySymbol(formatSymbol, sizeof(formatSymbol), format[0]);
    std::string result;
    wmkcSize index;

    if(!correctByteOrder && !correctFormatSymbol) {
        wmkcErr_exception(wmkcErr_Err32, "wmkcStruct::pack", "Incorrect format characters.");
    }
    if(format.size() <= args.size()) {
        wmkcErr_exception(wmkcErr_Err32, "wmkcStruct::pack", "The format characters do not correspond exactly one-to-one with the content.");
    }

    wmkcSize format_size = format.size();
    wmkcSize args_size = args.size();
    for(index = 0; index < args_size; ++index) {
        if(!verifySymbol(formatSymbol, sizeof(formatSymbol), format[index + 1])) {
            wmkcErr_exception(wmkcErr_Err32, "wmkcStruct::pack", "format error.");
        }
        
        // printf("content[%llu]: %c, %llu\n", index, format[index + 1], args[index]);
    }

    std::cout << "done." << std::endl;

    return result;
}

std::vector<wmkcSize> wmkcStruct::unpack(std::string format, std::string args)
{
    std::vector<wmkcSize> result;

    return result;
}

