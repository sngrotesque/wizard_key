// python make.py test\struct_test.cpp -O3 -Wall -DWUK_EXPORTS
#include <struct.cpp>
#include <misc.cpp>

#include <cstdarg>
#include <cmath>

#include <iostream>
#include <vector>
#include <any>

using namespace std;

class WUK_endian {
public:
    bool change_endian;

    WUK_endian(wuk::endianness specify)
    : change_endian(false)
    {
        wuk::endianness current = \
            (WUK_LE_ENDIAN)?(wuk::endianness::LE):(wuk::endianness::BE);
        this->change_endian = \
            ((specify!=wuk::endianness::NO)&&(current!=specify))?(true):(false);
    }

    void reverse(char *array, w_u32 length)
    {
        char swap;
        for(w_u32 i = 0; i < (length >> 1); ++i) {
            swap = array[i];
            array[i] = array[length - i - 1];
            array[length - i - 1] = swap;
        }
    }

    template <typename T>
    void switch_endianness(char *array, T arg)
    {
        memcpy(array, &arg, sizeof(T));
        if(change_endian) {
            reverse(array, sizeof(T));
        }
    }
};

int main(int argc, char **argv)
{
    WUK_endian ctx{wuk::endianness::BE};

    w_byte buffer[8]{};
    double number = 3.1415926;

    ctx.switch_endianness(reinterpret_cast<char *>(buffer), number);

    wuk::misc::print_hex(buffer, sizeof(number), 8, 0, 0);

    return 0;
}
