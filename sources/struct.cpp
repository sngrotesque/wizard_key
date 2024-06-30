#include <struct.hpp>

template <typename T>
wuk::FormatArgs wuk::Struct::format_string_parser(const std::string formatString, T arg)
{
    const char *fmt_ptr = formatString.c_str();
    char buffer_bytearray[8]{};
    FormatArgs result{};

    if(!isdigit(*fmt_ptr)) {
        // 如果第一个字符不是数字
        result.count = 1;
    } else {
        // 否则
        result.count = static_cast<wU32>(strtoul(fmt_ptr, nullptr, 10));
        for(; *fmt_ptr && isdigit(*fmt_ptr); fmt_ptr++);
    }

    switch(*fmt_ptr) {
    case 'x':
        result.type = formatType::FMT_PAD;
        break;
    case 'c':
    case 'b':
        result.type = formatType::FMT_SC;
        break;
    case 'B':
        result.type = formatType::FMT_UC;
        break;
    case '?':
        result.type = formatType::FMT_BOOL;
        break;
    case 'h':
        result.type = formatType::FMT_SSH;
        break;
    case 'H':
        result.type = formatType::FMT_USH;
        break;
    case 'i':
        result.type = formatType::FMT_SI;
        break;
    case 'I':
        result.type = formatType::FMT_UI;
        break;
    case 'l':
        result.type = formatType::FMT_SL;
        break;
    case 'L':
        result.type = formatType::FMT_UL;
        break;
    case 'q':
    case 'n':
        result.type = formatType::FMT_SQ;
        break;
    case 'Q':
    case 'N':
        result.type = formatType::FMT_UQ;
        break;
    case 'f':
        result.type = formatType::FMT_F;
        break;
    case 'd':
        result.type = formatType::FMT_D;
        break;
    case 's':
        result.type = formatType::FMT_S;
        break;
    case 'p':
        result.type = formatType::FMT_P;
        break;
    default:
        result.type = formatType::FMT_ERR;
        break;
    }

    return result;
}

