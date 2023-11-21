#include <crypto/snc.hpp>

#include <wmkc_random.hpp>
#include <wmkc_misc.hpp>

using namespace wmkcCrypto;
using namespace std;

static const wmkcByte key[96] = {
    0x13, 0x3a, 0xa9, 0xd2, 0xc8, 0x85, 0xaf, 0x35, 0xde, 0x00, 0xa7, 0x8b, 0x5d, 0xd5, 0x65, 0xa9,
    0xc5, 0x88, 0x1f, 0xb0, 0xb9, 0x02, 0xba, 0xce, 0xbb, 0x9a, 0x28, 0xc6, 0x3f, 0xed, 0xdb, 0x03,
    0xa0, 0x8a, 0x41, 0x05, 0xa2, 0x29, 0xbb, 0x30, 0x8b, 0x5a, 0x9f, 0xf5, 0x36, 0x5b, 0xa6, 0xa8,
    0x18, 0x93, 0x3c, 0x5d, 0x63, 0x9c, 0xe7, 0x32, 0x78, 0xcd, 0xf1, 0xb8, 0xa9, 0xbd, 0xd0, 0xdf,
    0xe3, 0x11, 0x92, 0xe0, 0x54, 0x4a, 0x58, 0x4b, 0x33, 0x61, 0xbb, 0xe1, 0xe4, 0x1a, 0x28, 0x80,
    0x91, 0x64, 0xc9, 0x0f, 0x94, 0xd1, 0x2c, 0x84, 0xb4, 0xbf, 0xd4, 0xf2, 0xc3, 0xd8, 0x80, 0x65};
static const wmkcByte iv[32] = {
    0x1a, 0x39, 0xd9, 0x58, 0xf1, 0x50, 0x62, 0x17, 0x7c, 0x15, 0xe6, 0x19, 0x9e, 0x08, 0xe6, 0xa8,
    0x71, 0x23, 0x99, 0x17, 0xe1, 0xdb, 0x7b, 0xcf, 0x3a, 0xf1, 0xe4, 0x29, 0xf2, 0x58, 0x6e, 0x00};

int main()
{
    wmkcSNC snc(key, iv);
    wmkcChar _tmp[33] = {"hello,world....................."};
    wmkcByte *buf = (wmkcByte *)_tmp;
    wmkcSize size = strlen(_tmp);

    snc.encrypt(buf, size, SNC_XcryptMode::CFB);

    wmkcMisc::PRINT(buf, size, 16, 1, 0);

    return 0;
}
