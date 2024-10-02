#include <config/wuk.hpp>
#include <config/exception.hpp>

#include <iostream>

#include <any>
#include <vector>

#include <cstdarg>

namespace struct_impl {
    enum class endianness {no, le, be};

    class BytesBuffer {
    public:
        w_byte *buffer;
        w_ulong length;
    };

    class Struct {
    private:
        bool is_switch_endianness;

    public:
        Struct()
        : is_switch_endianness(false)
        {

        }

        BytesBuffer pack(std::string format_string, ...)
        {
            const char *fmt_ptr = format_string.c_str();

            endianness current = \
                (WUK_LE_ENDIAN)?(endianness::le):(endianness::be);
            endianness specify = endianness::no;
            BytesBuffer result{};

            switch(*fmt_ptr) {
                case '>':
                case '!':
                    specify = endianness::be;
                    goto skip_ctrl_format;
                case '<':
                    specify = endianness::le;
                    goto skip_ctrl_format;
                case '@':
                    goto skip_ctrl_format;
                case '=':
                    skip_ctrl_format:
                    fmt_ptr++;
            }

            this->is_switch_endianness = \
                ((specify!=endianness::no)&&(current!=specify))?(true):(false);

            va_list va;
            va_start(va, format_string);
            // va_arg(va, int);
            va_end(va);

            return result;
        }
    };
}

template <typename T>
void handler(char format, w_size arg_count, std::vector<T> args)
{
    printf("format: %c, arg_count: %zu.\n", format, arg_count);
    for(auto const &i : args) {
        printf("item: %d\n", i);
    }
}

template <typename T>
std::vector<T> func(std::vector<std::any> args, w_size arg_count, w_u32 &args_index)
{
    std::vector<T> copy_args;

    for(w_u32 _ = 0; _ < arg_count; ++_) {
        copy_args.push_back(std::any_cast<T>(args.at(args_index)));
        args_index++;
    }

    return copy_args;
}

void sub_pack(char format, w_size arg_count, std::vector<std::any> args, w_u32 &args_index)
{
    switch(format) {
        case 'x':
            break;
        case 'c':
        case 'b':
            func<char>(args, arg_count, args_index); break;
        case 'B':
            func<w_byte>(args, arg_count, args_index); break;
        case '?':
            func<bool>(args, arg_count, args_index); break;
        case 'h':
            func<w_i16>(args, arg_count, args_index); break;
        case 'H':
            func<w_u16>(args, arg_count, args_index); break;
        case 'i':
            func<w_i32>(args, arg_count, args_index); break;
        case 'I':
            func<w_u32>(args, arg_count, args_index); break;
        case 'l':
            func<w_i64>(args, arg_count, args_index); break;
        case 'L':
            func<w_u64>(args, arg_count, args_index); break;
        case 'q':
        case 'n':
            func<w_ssize>(args, arg_count, args_index); break;
        case 'Q':
        case 'N':
            func<w_size>(args, arg_count, args_index); break;
        case 'f':
            func<float>(args, arg_count, args_index); break;
        case 'd':
            func<double>(args, arg_count, args_index); break;
        case 's':
        case 'p':
        default:
            throw wuk::Exception(wukErr_Err, "wuk::Struct::format_parser",
                "Incorrect formatting character.");
            break;
    }
}

void pack(const char* format_string, std::vector<std::any> args)
{
    w_u32 loopIndex = 0;
    w_u32 argsIndex = 0;

    wSize arg_count;
    char this_format;

    while (format_string[loopIndex] != 0) {
        char current_char = format_string[loopIndex];

        if (std::isdigit(current_char)) {
            // 如果是数字，解析出参数个数
            arg_count = 0;
            while(std::isdigit(format_string[loopIndex])) {
                arg_count = arg_count * 10 + (format_string[loopIndex]) - '0';
                ++loopIndex;
            }
            this_format = format_string[loopIndex];

            // // 在此处判断元素类型并传入handler函数
            // std::vector<int> copy_args;
            // for(w_u32 _ = 0; _ < arg_count; ++_) {
            //     copy_args.push_back(std::any_cast<int>(args.at(argsIndex)));
            //     argsIndex++;
            // }

            // handler(this_format, arg_count, copy_args);
        } else {
            // 否则，统计连续相同字符的个数
            arg_count = 1;
            while (format_string[loopIndex + 1] == current_char) {
                ++arg_count;
                ++loopIndex;
            }
            this_format = current_char;

            // // 在此处判断元素类型并传入handler函数
            // std::vector<int> copy_args;
            // for(w_u32 _ = 0; _ < arg_count; ++_) {
            //     copy_args.push_back(std::any_cast<int>(args.at(argsIndex)));
            //     argsIndex++;
            // }

            // handler(this_format, arg_count, copy_args);
        }

        ++loopIndex;
    }
}

/* 这个JB库实现起来真tm难啊！！！！ */

int main(int argc, char **argv)
{
    pack("3I4c2H", {3, 3, 3, 51, 51, 51, 51, 17, 17});

    return 0;
}
