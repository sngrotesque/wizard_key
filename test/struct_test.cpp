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
    wuk::FormatArgs FArgs;

    // Struct.is_switch_endianness = true;
    Struct.pack("!I", {});
    try {
        FArgs = Struct.format_string_parser("2B", vector<double>{0x1b, 0xf1, 0x7f, 'B'});

        if(FArgs.result.empty()) {
            cout << "result is null." << endl;
            return;
        }
        wByte *buffer = reinterpret_cast<wByte *>(FArgs.result.data());
        wSize  length = FArgs.result.size();

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
    while (format_string[i]) {
        char current_char = format_string[i];
        if (std::isdigit(current_char)) {
            // 如果是数字，解析出参数个数
            int count = 0;
            while (std::isdigit(format_string[i])) {
                count = count * 10 + (format_string[i] - '0');
                ++i;
            }
            char format = format_string[i];
            B(format, count);
        } else {
            // 否则，统计连续相同字符的个数
            int count = 1;
            while (format_string[i + 1] == current_char) {
                ++count;
                ++i;
            }
            B(current_char, count);
        }
        ++i;
    }
}

int main(int argc, char **argv)
{
    // struct_test();

    // const char *format_string_test1 = "IHccccB3Qd4x";
    // const char *format_string_test2 = "IIIIIIc256xccbQ";
    // const char *format_string_test3_1 = "IIccBQQQdf32x";
    // const char *format_string_test3_2 = "2I2c1B3Q1d1f32x";
    // const char *format_string_test3_3 = "2I2cB3Qdfxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    // A(format_string_test3_3);

    stringstream ss;
    ss << "function" << "[" << to_string(23) << "]:" << "message";
    string res;

    ss >> res;

    cout << res << endl;

    return 0;
}
