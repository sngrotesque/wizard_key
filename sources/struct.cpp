/**
 * 目前的实现只为了最根本的先实现这个库，对于优化方面，等这个库完全实现之后再进行。
 * 
 * 目前的实现不包括'@'字节顺序，大小和对齐方式。
 */

#include <struct.hpp>

/* -------------------------- Private ------------------------------ */

template <typename T>
void wuk::Struct::switch_endianness(char *buffer, T arg)
{
    memcpy(buffer, &arg, sizeof(T));
    if(this->is_switch_endianness) {
        this->reverse_array(buffer, sizeof(T));
    }
}

void wuk::Struct::reverse_array(char *array, w_u32 size)
{
    for(w_u32 i = 0; i < (size >> 1); ++i) {
        char swap = array[i];
        array[i] = array[size - i - 1];
        array[size - i - 1] = swap;
    }
}

std::string wuk::Struct::format_x_option(wSize length)
{
    char *result = new (std::nothrow) char[length];
    if(!result) {
        throw wuk::Exception(wukErr_ErrMemory, "wuk::Struct::format_x_option",
            "Failed to allocate memory for result.");
    }

    wuk::memory_zero(result, length);

    std::string result_string{result, length};
    delete[] result;

    return result_string;
}

/**
 * 此处不直接使用std::string result_string(args.begin(), args.end());的原因是为了兼容后续
 * 会出现的'@'字节序规则。
 * 
 * 此方法不应该检查传入的参数个数是否与格式字符串相对应，请在上级方法中进行检查。
 */
template <typename T>
std::string wuk::Struct::foramt_common_option(std::vector<T> args)
{
    wSize length = sizeof(T) * args.size();
    char *result = new (std::nothrow) char[length];
    if(!result) {
        throw wuk::Exception(wukErr_ErrMemory, "wuk::Struct::format_x_option",
            "Failed to allocate memory for result.");
    }
    wuk::memory_zero(result, length);

    // 在此函数中决定是否切换端序
    std::string result_string{};
    wSize ri, ai; // result index, args index.
    try {
        for(ri = ai = 0; ri < length; ri += sizeof(T), ++ai) {
            *(result + ri) = args.at(ai);
        }
    } catch (std::exception &e) {
        delete[] result;
        throw;
    }

    result_string = std::string{result, length};

    delete[] result;
    return result_string;
}

/* -------------------------- Public ------------------------------- */

wuk::Struct::Struct()
: is_switch_endianness(false)
{
    
}

/**
 * @brief 格式字符串解析函数
 * @authors SN-Grotesque
 * @note 后续考虑将数量作为此函数参数进行传入
 * @param formatString 传入时必须是一个子串，比如母串是"I3H5BQQQQd5xx"，
 *      那么每次传入的参数分别必须是"I", "3H", "5B", "Q", "Q", "Q", "Q", "d", "5x", "x"。
 * @param args 对应于formatString类型的值，如果formatString当前为[x]，那么必须默认为0。
 * @return 无
 */
template <typename T>
wuk::FormatArgs wuk::Struct::format_string_parser(std::string formatString, std::vector<T> arg)
{
    const char *fmt_ptr = formatString.c_str();
    // char buffer_bytearray[8]{};
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
        result.type   = formatType::FMT_PAD;
        result.result = this->format_x_option(result.count);
        break;
    case 'c':
    case 'b':
        result.type = formatType::FMT_SC;
        result.result = this->foramt_common_option(arg);
        break;
    case 'B':
        result.type = formatType::FMT_UC;
        result.result = this->foramt_common_option(arg);
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

std::string wuk::Struct::pack(std::string format_string, std::vector<std::any> args)
{
    // 获取当前字节顺序
    wuk::endianness current = \
        (WUK_LE_ENDIAN)?(wuk::endianness::LE):(wuk::endianness::BE);;
    // 将指定字节顺序初始化为无
    wuk::endianness specify = wuk::endianness::NO;

    const char *fmt_ptr = format_string.c_str();
    switch (*fmt_ptr) {
    // 大端序（网络序）
    case '>':
    case '!':
        specify = wuk::endianness::BE;
        goto skip_ctrl_symbol;
    // 小端序
    case '<':
        specify = wuk::endianness::LE;
        goto skip_ctrl_symbol;
    /* 字节顺序-大小-对齐方式：按原字节。
    *  此字节序方法有额外实现，后续再实现 */
    case '@':
        goto skip_ctrl_symbol;
    case '=':
    skip_ctrl_symbol:
        fmt_ptr++;
    default:
        break;
    }

    this->is_switch_endianness = \
        ((specify!=wuk::endianness::NO)&&(current!=specify))?(true):(false);

    return std::string();
}

std::vector<std::any> wuk::Struct::unpack(std::string format_string, std::string buffer)
{

    return std::vector<std::any>();
}
