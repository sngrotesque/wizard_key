// python make.py test\struct_test.cpp -O3 -Wall -DWUK_EXPORTS
#include <struct.cpp>
#include <time.cpp>
#include <misc.cpp>

#include <cstdarg>
#include <cmath>

#include <iostream>
#include <sstream>
#include <vector>
#include <any>

#ifdef WUK_STD_CPP_20
#   include <format>
#endif

using namespace std;

void struct_test()
{
    wuk::Struct Struct;

    Struct.pack("!I", {});
    try {
        vector<char> args{'A','b','C','d'};
        wSize args_size = args.size();
        printf("param count: %zu.\n", args_size);
        string result = Struct.format_parser('x', 5, vector<bool>{});

        if(result.empty()) {
            cout << "result is null." << endl;
            return;
        }
        wByte *buffer = reinterpret_cast<wByte *>(result.data());
        wSize  length = result.size();

        cout << "Hex print:\n";
        wuk::misc::print_hex(buffer, length, 16, 1, 0);

        cout << "\nPyBytes print:\n";
        wuk::misc::print_pybytes(buffer, length, true);
    } catch (exception &e) {
        cout << "Error: " << e.what() << endl;
    }
}

void B(char fmt_char, w_u32 count)
{
#ifdef WUK_STD_CPP_20
    cout << format("B('{0}', {1})", fmt_char, count) << endl;
#else
    std::cout << "B('" << fmt_char << "', " << count << ")" << std::endl;
#endif
}

void A(const char* format_string)
{
    w_u32 i = 0;
    wSize count;
    while (format_string[i] != 0) {
        char current_char = format_string[i];
        if (std::isdigit(current_char)) {
            // 如果是数字，解析出参数个数
            count = 0;
            while (std::isdigit(format_string[i])) {
                count = count * 10 + (format_string[i] - 0x30);
                ++i;
            }
            char format = format_string[i];
            B(format, count);
        } else {
            // 否则，统计连续相同字符的个数
            count = 1;
            while (format_string[i + 1] == current_char) {
                ++count;
                ++i;
            }
            B(current_char, count);
        }
        ++i;
    }
}

class LIBWUK_API BinaryBuffer {
public:
    wByte *buffer;
    wSize  length;
};

class LIBWUK_API Struct_impl {
public:
    bool is_switch_endianness;
    enum class endianness {NO, LE, BE};

    void reverse_array(char *array, wU32 size)
    {
        wU32 i;
        for(i = 0; i < (size >> 1); ++i) {
            char swap = array[i];
            array[i] = array[size - i - 1];
            array[size - i - 1] = swap;
        }
    }

    // 这个函数的第三个参数应改为`vector<T> args`
    void print_format_args(char format, wSize count, vector<any> args)
    {
        cout << "format: " << format << endl;
        cout << "count:  " << count  << endl;

        for(wSize i = 0; i < args.size(); ++i) {
            switch(format) {
            case 'x':
                break;
            case 'c':
            case 'b':
                cout << "test: c, b" << endl;
                cout << any_cast<char>(args[i]) << endl; break;
            case 'B':
                cout << "test: B" << endl;
                cout << any_cast<wByte>(args[i]) << endl; break;
            case '?':
                cout << "test: ?" << endl;
                cout << any_cast<bool>(args[i]) << endl; break;
            case 'h':
                cout << "test: h" << endl;
                cout << any_cast<wI16>(args[i]) << endl; break;
            case 'H':
                cout << "test: H" << endl;
                cout << any_cast<wU16>(args[i]) << endl; break;
            case 'i':
                cout << "test: i" << endl;
                cout << any_cast<wI32>(args[i]) << endl; break;
            case 'I':
                cout << "test: I" << endl;
                cout << any_cast<wU32>(args[i]) << endl; break;
            case 'l':
                cout << "test: l" << endl;
                cout << any_cast<wI64>(args[i]) << endl; break;
            case 'L':
                cout << "test: L" << endl;
                cout << any_cast<wU64>(args[i]) << endl; break;
            case 'q':
            case 'n':
                cout << "test: q, n" << endl;
                cout << any_cast<wSSize>(args[i]) << endl; break;
            case 'Q':
            case 'N':
                cout << "test: Q, N" << endl;
                cout << any_cast<wSize>(args[i]) << endl; break;
            case 'f':
                cout << "test: f" << endl;
                cout << any_cast<float>(args[i]) << endl; break;
            case 'd':
                cout << "test: d" << endl;
                cout << any_cast<double>(args[i]) << endl; break;
            case 's':
            case 'p':
                cout << "test: s, p" << endl;
                // 其他实现
                break;
            default:
                throw wuk::Exception(wukErr_Err, "wuk::Struct::format_parser",
                    "Incorrect formatting character.");
            }
        }
    }

    // 传入print_format_args方法的第三个参数应给出在此方法中的对应索引起始位
    void format_string_parser(const char *format_string, vector<any> args)
    {
        wU32  i = 0;
        wSize count;
        char this_ch;

        while(format_string[i] != 0) {
            printf("format_string_parser::index::%u.\n", i);
            this_ch = format_string[i];
            if (isdigit(this_ch)) {
                count = 0;
                while(isdigit(format_string[i])) {
                    count = count * 10 + (format_string[i] - '0');
                    ++i;
                }
                this->print_format_args(format_string[i], count, args);
            } else {
                count = 1;
                while(format_string[i + 1] == this_ch) {
                    ++count;
                    ++i;
                }
                this->print_format_args(this_ch, count, args);
            }
            ++i;
        }
    }

    BinaryBuffer pack(const char *format_string, vector<any> args)
    {
        endianness current = (WUK_LE_ENDIAN)?(endianness::LE):(endianness::BE);
        endianness specify = endianness::NO;
        BinaryBuffer result;

        const char *fmt_ptr = format_string;
        switch(*fmt_ptr) {
        case '>':
        case '!':
            specify = endianness::BE;
            goto skip_ctrl_symbol;
        case '<':
            specify = endianness::LE;
            goto skip_ctrl_symbol;
        case '@':
            goto skip_ctrl_symbol;
        case '=':
            skip_ctrl_symbol:
            fmt_ptr++;
        default:
            break;
        }

        this->is_switch_endianness = \
            ((specify!=endianness::NO)&&(current!=specify))?(true):(false);

        format_string_parser(fmt_ptr, args);

        return result;
    }
};

int main(int argc, char **argv)
{
    // struct_test();

    // const char *format_string_test1 = "IHccccB3Qd4x";
    // const char *format_string_test2 = "IIIIIIc256xccbQ";
    // const char *format_string_test3_1 = "IIccBQQQdf32x";
    // const char *format_string_test3_2 = "2I2c1B3Q1d1f32x";
    // const char *format_string_test3_3 = "2I2cB3Qdfxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    // A(format_string_test3_3);

    Struct_impl Struct;
    Struct.pack("!If5sB", {1U, 3.3f, "hello", 0x89});

    return 0;
}
