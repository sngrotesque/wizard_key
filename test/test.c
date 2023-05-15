#include <image/snPng.h>
#include <crypto/snc.h>
#include <crypto/snt.h>
#include <snPadding.h>
#include <snMisc.h>
#include <snTime.h>
#include <snRand.h>
#include <snNum.h>

#include <image/snPng.c>
#include <crypto/snc.c>
#include <crypto/snt.c>
#include <snPadding.c>
#include <snMisc.c>
#include <snTime.c>
#include <snRand.c>
#include <snNum.c>

static snChar key[96] = {
    "10o34809284098403148491749134133"
    "jnzcuy139ry9asu0835houjfaljsdljh"
    "139-u-d9sadpj315kms;dkfspi183534"};
static snChar iv[32] = {
    "bijdlishr3197gas0duh315ipjsipjfa"};

void test()
{
    // static snChar buf[32];
    SNC_ctx *snc = snNull;
    SNC_new(&snc, SNC_768);
    SNC_init(snc, (snByte *)key, (snByte *)iv);

    // SNC_CBC_Encrypt(snc, (snByte *)buf, 32);
    // snMisc_PRINT((snByte *)buf, 32, 16, 0, 0);

    snMisc_PRINT(snc->roundKey, sizeof(snc->roundKey), 32, sizeof(snc->roundKey) % 32, 0);

    SNC_release(&snc);
}

int main(int argc, char **argv)
{
    // speed_test();
    test();

    return 0;
}
