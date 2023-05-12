#include <image/snPng.h>
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

int main(int argc, char **argv)
{
    static snByte array[8];
    snSize number = 256;
    snError code;

    code = snNum_pack(">Q", array, &number);

    printf("错误代码：%x\n", code);

    snMisc_PRINT(array, sizeof(array), sizeof(array), 0, 0);

    return 0;
}
