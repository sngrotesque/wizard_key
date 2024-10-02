/**
 * 目前的实现只为了最根本的先实现这个库，对于优化方面，等这个功能完全实现之后再进行。
 * 
 * 目前的实现不包括'@'字节顺序，大小和对齐方式。
 */

#include <WukStruct.hh>

/* -------------------------- Private ------------------------------ */

/**
 * @brief 翻转缓冲区
 * @authors SN-Grotesque
 * @param array 指向缓冲区的指针
 * @param size 缓冲区长度（单位：字节）
 * @return 无
 */
void wuk::Struct::reverse_array(char *array, w_u32 size)
{
    for(w_u32 i = 0; i < (size >> 1); ++i) {
        char swap = array[i];
        array[i] = array[size - i - 1];
        array[size - i - 1] = swap;
    }
}

/**
 * @brief 此方法用于将传入的参数编码为二进制数据。
 * @authors SN-Grotesque
 * @note 此方法不应该检查传入的参数个数是否与格式字符串相对应，请在上级方法中进行检查。
 * @param args 需要编组的同类型数据向量
 * @return 编组后的二进制序列
 */
template <typename T>
std::string wuk::Struct::foramt_number_option(std::vector<T> args)
{
    wSize ri; // result index
    wSize ai; // args index.

    // 申请一个缓冲区用于存放接下来要编组的二进制数据
    wSize length = sizeof(T) * args.size();
    char *result = new (std::nothrow) char[length];
    if(!result) {
        throw wuk::Exception(wukErr_ErrMemory, "wuk::Struct::format_x_option",
            "Failed to allocate memory for result.");
    }
    wuk::memory_zero(result, length);

    for(ri = ai = 0; ri < length; ri += sizeof(T), ++ai) {
        /**
         * 将值复制到缓冲区对应位置，此处不应该用'='号赋值，因为会发生截断。
         * 由于std::vector.at方法会检测是否越界，所以需要捕获异常并进行处理。
         */
        try {
            // 创建一个临时变量的目的是为了兼容bool类型，不然可以直接`&args.at(ai)`。
            T temp = args.at(ai);
            memcpy(result + ri, &temp, sizeof(T));
            if(this->is_switch_endianness) {
                this->reverse_array(result + ri, sizeof(T));
            }
        } catch (std::exception &e) {
            delete[] result;
            throw;
        }
    }

    std::string result_string{result, length};
    delete[] result;
    return result_string;
}

/* -------------------------- Public ------------------------------- */

wuk::Struct::Struct()
{
    this->is_switch_endianness = false;
}

template <typename T>
std::string wuk::Struct::format_parser(char fmt_char, wSize count, std::vector<T> args)
{
    std::string result;

    switch(fmt_char) {
    case 'x': 
        result.append(count, 0x00);
        break;
    case 'c':
    case 'b':
    case 'B':
    case '?':
    case 'h':
    case 'H':
    case 'i':
    case 'I':
    case 'l':
    case 'L':
    case 'q':
    case 'n':
    case 'Q':
    case 'N':
    case 'f':
    case 'd':
        result = this->foramt_number_option(args);
        break;
    case 's':
    case 'p':
        // 其他实现
        break;
    default:
        throw wuk::Exception(wukErr_Err, "wuk::Struct::format_parser",
            "Incorrect formatting character.");
    }

    return result;
}

// 现在我太困了，明天再继续写吧。
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
