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

    FArgs = Struct.format_string_parser("256x", 0);

    if(FArgs.result.empty()) {
        cout << "result is null." << endl;
        return;
    }
    wByte *buffer = reinterpret_cast<wByte *>(FArgs.result.data());
    wSize  length = FArgs.result.size();

    // wuk::misc::print_hex(buffer, length, 16, 1, 0);
    wuk::misc::print_pybytes(buffer, length, true);
}

int main(int argc, char **argv)
{
    struct_test();
    return 0;
}
