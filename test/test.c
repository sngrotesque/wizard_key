// #include <network/wmkc_net.h>
// #include <image/wmkc_png.h>
// #include <crypto/snc.h>
// #include <wmkc_binascii.h>
// #include <wmkc_hexdump.h>
// #include <wmkc_object.h>
// #include <wmkc_random.h>
// #include <wmkc_struct.h>
// #include <wmkc_thread.h>
// #include <wmkc_base64.h>
// #include <wmkc_dict.h>
// #include <wmkc_file.h>
// #include <wmkc_misc.h>
// #include <wmkc_math.h>
// #include <wmkc_time.h>
// #include <wmkc_hash.h>
// #include <wmkc_pad.h>
// #include <wmkc_key.h>

// #include <network/wmkc_net.c>
#include <image/wmkc_png.c>
#include <crypto/snc.c>
#include <wmkc_binascii.c>
#include <wmkc_hexdump.c>
#include <wmkc_object.c>
#include <wmkc_random.c>
#include <wmkc_struct.c>
#include <wmkc_thread.c>
#include <wmkc_base64.c>
#include <wmkc_dict.c>
#include <wmkc_file.c>
#include <wmkc_misc.c>
#include <wmkc_math.c>
#include <wmkc_time.c>
#include <wmkc_hash.c>
#include <wmkc_pad.c>
#include <wmkc_key.c>

#define CIPHER_TEST true

#if defined(WMKC_SNC) && (CIPHER_TEST)
static wmkcByte testKey[96] = {
    0x67, 0x65, 0x5b, 0x7b, 0x33, 0x78, 0x74, 0x6e, 0x49, 0x62, 0x6d, 0x3b, 0x7a, 0x77, 0x5d, 0x53,
    0x71, 0x52, 0x4a, 0x30, 0x3e, 0x2e, 0x63, 0x24, 0x67, 0x3b, 0x6e, 0x61, 0x6b, 0x3a, 0x21, 0x73,
    0x30, 0x66, 0x6c, 0x23, 0x74, 0x76, 0x45, 0x5d, 0x21, 0x3d, 0x50, 0x79, 0x60, 0x6c, 0x5d, 0x75,
    0x70, 0x6a, 0x68, 0x66, 0x52, 0x6e, 0x55, 0x4c, 0x7c, 0x51, 0x4e, 0x28, 0x31, 0x72, 0x7a, 0x40,
    0x49, 0x4e, 0x3b, 0x36, 0x6b, 0x69, 0x54, 0x37, 0x36, 0x70, 0x30, 0x3e, 0x3d, 0x60, 0x73, 0x42,
    0x5f, 0x61, 0x6f, 0x79, 0x7a, 0x36, 0x5e, 0x6d, 0x35, 0x40, 0x5f, 0x6d, 0x63, 0x57, 0x49, 0x31};
static wmkcByte testIv[32] = {
    0x7b, 0x75, 0x76, 0x37, 0x24, 0x59, 0x72, 0x29, 0x6e, 0x40, 0x5f, 0x65, 0x2d, 0x55, 0x75, 0x68,
    0x3d, 0x41, 0x78, 0x35, 0x48, 0x50, 0x7d, 0x73, 0x60, 0x4e, 0x33, 0x6f, 0x23, 0x47, 0x4c, 0x36};
#endif

void test()
{
    wmkcChar text[2048] = {
        "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh"
        "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh"
    };
    wmkcByte *buf = (wmkcByte *)text;
    wmkcSize size = strlen(text);
    SNC_ctx *snc  = wmkcNull;
    wmkc_u32 encrypt_method = 2;
    wmkcSize group = 32;

    SNC_new(&snc, SNC_768);
    SNC_init(snc, testKey, testIv);

    if(size % SNC_BLOCKLEN)
        wmkcPad_add(buf, &size, SNC_BLOCKLEN, false);

    printf("Cipher info:\n");
    printf("\tMode: SNC"WMKC_SET_COLOR(WMKC_FORE_COLOR_LIGHTRED, "%u, "), (snc->mode + 1) * 256);
    printf("\tKeySize: "WMKC_SET_COLOR(WMKC_FORE_COLOR_LIGHTCYAN, "%u, "),   snc->KN);
    printf("\tRounds:  "WMKC_SET_COLOR(WMKC_FORE_COLOR_LIGHTMAGENTA, "%u")"\n", snc->NR);

    printf("Plaintext:\n"); wmkcMisc_PRINT(buf, size, group, 0, true);
    printf("Key used: \n"); wmkcMisc_PRINT(snc->roundKey, snc->KN, group, 0, true);
    printf("Iv used:  \n"); wmkcMisc_PRINT(snc->iv, SNC_BLOCKLEN, group, 0, true);

    printf("Encrypt method:\n");
    if(encrypt_method == 1) {
        printf("\tECB (Electronic Codebook).\n");
        SNC_ECB_Encrypt(snc, buf, size);
    } else if(encrypt_method == 2) {
        printf("\tCBC (Cipher Block Chaining).\n");
        SNC_CBC_Encrypt(snc, buf, size);
    }

    printf("Ciphertext:\n"); wmkcMisc_PRINT(buf, size, group, 0, true);

    SNC_free(&snc);
}

int main(int argc, char **argv)
{
    test();
    return 0;
}
