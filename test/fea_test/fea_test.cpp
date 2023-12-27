#include <crypto/fea.hpp>

// #include <wmkc_random.hpp>
// #include <wmkc_time.hpp>

#include <wmkc_misc.hpp>

using namespace wmkcCrypto;
using namespace wmkcMisc;
using namespace std;

int main()
{
    wmkcByte data[16] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    wmkcByte key[32] = {
        0xb0, 0xba, 0x8f, 0xe4, 0xc4, 0x7c, 0xc3, 0x7d, 0xac, 0x18, 0x29, 0x15, 0xd8, 0xab, 0xcd, 0xa7,
        0x9f, 0x97, 0x65, 0xc4, 0x98, 0x96, 0xf0, 0x40, 0x9c, 0x5e, 0x5e, 0x43, 0xbc, 0xee, 0x2f, 0x90
    };
    wmkcByte iv[16] = {
        0x61, 0x90, 0xa9, 0x28, 0xa4, 0x11, 0x1c, 0x0b, 0x22, 0xf8, 0x66, 0xcd, 0xfc, 0x2b, 0xcd, 0xc1
    };

    wmkcFEA fea(key, iv);

    cout << "Key:\n";
    PRINT(key, sizeof(key), WMKC_FEA_BLOCKLEN, sizeof(key) % WMKC_FEA_BLOCKLEN, true);
    cout << "Iv:\n";
    PRINT(iv, sizeof(iv), WMKC_FEA_BLOCKLEN, sizeof(iv) % WMKC_FEA_BLOCKLEN, true);
    cout << "Plaintext:\n";
    PRINT(data, sizeof(data), WMKC_FEA_BLOCKLEN, sizeof(data) % WMKC_FEA_BLOCKLEN, true);
    fea.encrypt(data, sizeof(data), FEA_XcryptMode::ECB);
    cout << "Ciphertext:\n";
    PRINT(data, sizeof(data), WMKC_FEA_BLOCKLEN, sizeof(data) % WMKC_FEA_BLOCKLEN, true);

    return 0;
}



