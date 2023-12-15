#include <network/wmkc_net.hpp>
#include <network/wmkc_ssl.hpp>
#include <network/wmkc_dns.hpp>

#include <crypto/snc.hpp>

#include <wmkc_binascii.hpp>
#include <wmkc_base64.hpp>

#include <wmkc_random.hpp>
#include <wmkc_struct.hpp>

#include <wmkc_hash.hpp>
#include <wmkc_misc.hpp>
#include <wmkc_time.hpp>

using namespace std;
using namespace wmkcNet;
using namespace wmkcCrypto;

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

int main()
{
    wmkcStruct Struct;

    try {
        Struct.pack(">BIIIH2", {0x45, 5, 4, 5, 12345, 0x3135});
    } catch(exception &e) {
        cout << e.what() << endl;
    }

    return 0;
}
