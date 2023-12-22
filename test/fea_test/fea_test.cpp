#include <crypto/fea.hpp>

#include <wmkc_random.hpp>
#include <wmkc_misc.hpp>
#include <wmkc_time.hpp>

using namespace wmkcCrypto;
using namespace wmkcMisc;
using namespace std;

int main()
{
    wmkcRandom random;
    wmkcTime time;
    wmkcFEA fea;

    wmkcByte key[32] = {
        0xb1, 0xba, 0x8f, 0xe4, 0xc4, 0x7c, 0xc3, 0x7d, 0xac, 0x18, 0x29, 0x15, 0xd8, 0xab, 0xcd, 0xa7,
        0x9f, 0x97, 0x65, 0xc4, 0x98, 0x96, 0xf0, 0x40, 0x9c, 0x5e, 0x5e, 0x43, 0xbc, 0xee, 0x2f, 0x90
    };
    wmkcByte iv[16] = {
        0x60, 0x90, 0xa9, 0x28, 0xa4, 0x11, 0x1c, 0x0b, 0x22, 0xf8, 0x66, 0xcd, 0xfc, 0x2b, 0xcd, 0xc1
    };

    fea.keyExtension(key, iv);

    wmkcBool total = 0;
    if(total) {
        for(int offset = 0; offset < sizeof(fea.roundKey); offset += 32) {
            PRINT(fea.roundKey + offset, 32, 16, 1, 0);
        }
    } else {
        PRINT(fea.roundKey + 32, 32, 16, 1, 0);
    }

    return 0;
}



