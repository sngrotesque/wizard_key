// #include <network/wmkc_net.hpp>
// #include <network/wmkc_ssl.hpp>
// #include <network/wmkc_dns.hpp>

// #include <crypto/snc.hpp>

// #include <wmkc_binascii.hpp>
// #include <wmkc_base64.hpp>

// #include <wmkc_random.hpp>
#include <wmkc_struct.hpp>

// #include <wmkc_hash.hpp>
#include <wmkc_misc.hpp>
// #include <wmkc_time.hpp>

#include <lzma.h>
#include <lzmalib.h>

using namespace std;

void va_test(string format, ...)
{
    va_list va;

    va_start(va, format);

    for(const auto &i : format) {
        if(i == 'I') {
            printf("32bit value: %d\n", va_arg(va, int));
        } else if(i == 'H') {
            printf("16bit value: %d\n", va_arg(va, int));
        }
    }
}

void struct_test()
{
    wmkcStruct Struct;

    string format = ">HHI";
    vector<wmkcSize> args = {
        0x1234, // ID
        0x0134, // Length
        0x00000003  // test
    };

    try {
        string res = Struct.pack(format, args);
        printf("Struct.orderSymbol: %d\n", Struct.orderSymbol);
        wmkcMisc::PRINT_RAW((wmkcByte *)(res.c_str()), res.size(), true);
    } catch(exception &e) {
        cout << e.what() << endl;
    }
}

int main()
{
    

    return 0;
}
