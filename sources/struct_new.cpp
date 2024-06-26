#include <struct_new.hpp>

wmkc::Struct::Struct()
{

}

std::string wmkc::Struct::pack(std::string format_string)
{
    const char *format = format_string.c_str();
    endianness current_endian = (WMKC_LE_ENDIAN)?(endianness::LE):(endianness::BE);
    endianness specify_endian = endianness::NO;
    endianness final_endian;

    switch (*format) {
    // 大端序
    case '>':
    case '!':
        specify_endian = wmkc::endianness::BE;
        goto endian_skip;
    // 小端序
    case '<':
        specify_endian = wmkc::endianness::LE;
        goto endian_skip;
    // 使用此计算机的默认端序
    case '@':
    case '=':
endian_skip:
        format++;
    default:
        break;
    }

    char this_ch;
    wU32 nargs;
    for(;;) {
        this_ch = *format++;
    }

    // printf("%p\n", format);
    printf("%s\n", format);

    return std::string();
}

// std::string wmkc::Struct::pack(std::string format, ...)
// {

// }

std::vector<std::any> wmkc::Struct::unpack(std::string format, const wByte *buffer, wSize length)
{

}

std::vector<std::any> wmkc::Struct::unpack(std::string format, std::string buffer)
{
    
}








