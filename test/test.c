#include <image/snPng.h>
#include <crypto/snc.h>
#include <crypto/snt.h>
#include <snPadding.h>
#include <snMisc.h>
#include <snMath.h>
#include <snTime.h>
#include <snRand.h>
#include <snNum.h>

#include <image/snPng.c>
#include <crypto/snc.c>
#include <crypto/snt.c>
#include <snPadding.c>
#include <snMisc.c>
#include <snMath.c>
#include <snTime.c>
#include <snRand.c>
#include <snNum.c>

static snChar key[96] = {
    "ge[{3xtnIbm;zw]SqRJ0>.c$g;nak:!s"
    "0fl#tvE]!=Py`l]upjhfRnUL|QN(1rz@"
    "IN;6kiT76p0>=`sB_aoyz6^m5@_mcWI1"};
static snChar iv[32] = {
    "{uv7$Yr)n@_e-Uuh=Ax5HP}s`N3o#GL6"};

void speed_test()
{
    static snByte buf[16777216];
    SNC_ctx *snc = snNull;
    snTime_ctx timer;

    SNC_new(&snc, SNC_768);
    SNC_init(snc, (snByte *)key, (snByte *)iv);

    snTime_TimerBegin(&timer);
    SNC_CBC_Encrypt(snc, buf, 16777216);
    snTime_TimerEnd(&timer);
    snTime_TimerPrint("time: ", &timer);

    SNC_release(&snc);
}

void test()
{
    static snByte buf[32] = {1};
    SNC_ctx *snc = snNull;

    SNC_new(&snc, SNC_768);
    SNC_init(snc, (snByte *)key, (snByte *)iv);

    SNC_CBC_Encrypt(snc, buf, 32);
    snMisc_PRINT(buf, 32, 4, 0, 0);

    SNC_release(&snc);
}

int main(int argc, char **argv)
{
    speed_test();
    // test();

    return 0;
}
