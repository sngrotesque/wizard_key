#include <crypto/fea.hpp>
#include <crypto/crypto.hpp>

#include <wmkc_base64.hpp>
#include <wmkc_random.hpp>
#include <wmkc_time.hpp>

#include <wmkc_misc.hpp>

#include <wmkc_pad.hpp>

using namespace wmkcCrypto;
using namespace wmkcMisc;
using namespace std;

static const wByte key[32] = {
    0xb0, 0xba, 0x8f, 0xe4, 0xc4, 0x7c, 0xc3, 0x7d, 0xac, 0x18, 0x29, 0x15, 0xd8, 0xab, 0xcd, 0xa7,
    0x9f, 0x97, 0x65, 0xc4, 0x98, 0x96, 0xf0, 0x40, 0x9c, 0x5e, 0x5e, 0x43, 0xbc, 0xee, 0x2f, 0x90
};
static const wByte iv[16] = {
    0x61, 0x90, 0xa9, 0x28, 0xa4, 0x11, 0x1c, 0x0b, 0x22, 0xf8, 0x66, 0xcd, 0xfc, 0x2b, 0xcd, 0xc1
};

void fea_test()
{
    wmkcFEA fea(key, iv);
    wChar text[2048] = {
        "GET / HTTP/1.1\r\n"
        "Host: passport.bilibili.com\r\n"
        "Acceot: */*\r\n"
        "Accept-Type: text/html\r\n"
        "User-Agent: Android\r\n"
        "\r\n"};
    wByte *buffer = (wByte *)text;
    wSize bufferSize = strlen(text);

    wmkcPad::pad(buffer, &bufferSize, WMKC_FEA_BLOCKLEN, false);
    cout << "Plaintext:\n"; PRINT(buffer, bufferSize, 16, (bufferSize % 16), 1);

    fea.cbc_encrypt(buffer, bufferSize);
    cout << "Ciphertext:\n"; PRINT(buffer, bufferSize, 16, (bufferSize % 16), 1);

    cout << "Ciphertext Base64: " << wmkcBase64().encode(string((char *)buffer, bufferSize)) << "\n";

    fea.cbc_decrypt(buffer, bufferSize);
    wmkcPad::unpad(buffer, &bufferSize);
    cout << "Plaintext:\n"; PRINT(buffer, bufferSize, 32, (bufferSize % 16), 1);
}

void fea_speed_test()
{
    wmkcFEA fea(key, iv);
    wSize length = 16777216;
    wByte *buffer = new wByte[length];

    wmkcTime time;

    double start = time.time();
    fea.cbc_encrypt(buffer, length);
    double stop = time.time();

    printf("time used: %.4lf\n", stop-start);

    delete[] buffer;
}

int main()
{
    fea_test();
    // fea_speed_test();
    return 0;
}
