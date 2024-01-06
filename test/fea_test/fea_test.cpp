#include <crypto/fea.hpp>
#include <crypto/crypto.hpp>

#include <wmkc_random.hpp>
#include <wmkc_time.hpp>

#include <wmkc_misc.hpp>

using namespace wmkcCrypto;
using namespace wmkcMisc;
using namespace std;

static const wmkcByte key[32] = {
    0xb1, 0xba, 0x8f, 0xe4, 0xc4, 0x7c, 0xc3, 0x7d, 0xac, 0x18, 0x29, 0x15, 0xd8, 0xab, 0xcd, 0xa7,
    0x9f, 0x97, 0x65, 0xc4, 0x98, 0x96, 0xf0, 0x40, 0x9c, 0x5e, 0x5e, 0x43, 0xbc, 0xee, 0x2f, 0x90
};
static const wmkcByte iv[16] = {
    0x60, 0x90, 0xa9, 0x28, 0xa4, 0x11, 0x1c, 0x0b, 0x22, 0xf8, 0x66, 0xcd, 0xfc, 0x2b, 0xcd, 0xc1
};

void create_sbox_rsbox()
{
    wmkcByte sbox[256], rsbox[256];

    generate_sbox(sbox);
    generate_rsbox(rsbox, sbox);

    PRINT_BOX(sbox, 256, 16, 1);

    PRINT_BOX(rsbox, 256, 16, 1);
}

void fea_test()
{
    wmkcChar text[256] = {"hello, world.31."};
    wmkcByte *data = (wmkcByte *)text;
    wmkcSize size = strlen(text);
    wmkcFEA fea(key, iv);

    cout << "Plaintext:\n"; PRINT(data, 16, 16, 1, 1);

    fea.cbc_encrypt(data, size);

    cout << "Ciphertext:\n"; PRINT(data, 16, 16, 1, 1);

    fea.cbc_decrypt(data, size);

    cout << "Plaintext:\n"; PRINT(data, 16, 16, 1, 1);
}

int main()
{
    fea_test();

    return 0;
}
