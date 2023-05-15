// #include <image/snPng.h>
#include <crypto/snc.h>
#include <snPadding.h>
#include <snMisc.h>
#include <snTime.h>
#include <snRand.h>
#include <snNum.h>

// #include <image/snPng.c>
#include <crypto/snc.c>
#include <snPadding.c>
#include <snMisc.c>
#include <snTime.c>
#include <snRand.c>
#include <snNum.c>

void speed_test()
{
    // 不需要做时间测试，不影响性能。
    sn_u32 arr[SNC_NB][SNC_NK] = {
        {0x00, 0x01, 0x02, 0x03},
        {0x04, 0x05, 0x06, 0x07},
        {0x08, 0x09, 0x0a, 0x0b},
        {0x0c, 0x0d, 0x0e, 0x0f},
        {0x10, 0x11, 0x12, 0x13},
        {0x14, 0x15, 0x16, 0x17},
        {0x18, 0x19, 0x1a, 0x1b},
        {0x1c, 0x1d, 0x1e, 0x1f},
    };
    sn_u32 *p = (sn_u32 *)arr;

    // 第7列第2排的元素。
    printf("%02x\n", *(p + ((SNC_NK * 7) + 2)));
}

void test()
{

}

int main(int argc, char **argv)
{
    speed_test();
    // test();

    return 0;
}
