// python make.py test\struct_test.cpp -O3 -Wall -DWUK_EXPORTS
#include <struct.cpp>
#include <time.cpp>
#include <misc.cpp>

#include <cstdarg>
#include <cmath>

#include <iostream>
#include <vector>
#include <any>

using namespace std;

void struct_test()
{
    wuk::Struct Struct;
    wuk::FormatArgs FArgs;

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

int main(int argc, char **argv)
{
    struct_test();

    return 0;
}
