#include <image/snPng.h>
#include <crypto/snc.h>
#include <snPadding.h>
#include <snMisc.h>
#include <snTime.h>
#include <snRand.h>
#include <snNum.h>

#include <zlib/zlib.h>

#include <image/snPng.c>
#include <crypto/snc.c>
#include <snPadding.c>
#include <snMisc.c>
#include <snTime.c>
#include <snRand.c>
#include <snNum.c>

int main(int argc, char **argv)
{
    static snByte IHDR[17 + 4];
    sn_u32 width, height, crc;
    snByte bitDeep, colorType;

    width = 0x00000002;
    height = 0x00000002;
    bitDeep = 0x08;
    colorType = 0x02;

    memcpy(IHDR, "IHDR", 4);
    snNum_pack(">I", IHDR + 4, width);
    snNum_pack(">I", IHDR + 8, height);
    IHDR[12] = bitDeep;
    IHDR[13] = colorType;

    crc = crc32(0, IHDR + 4, 13);
    snNum_pack(">I", IHDR + 4 + 13, crc);

    snMisc_PRINT(IHDR, sizeof(IHDR), sizeof(IHDR), 0, 0);



    return 0;
}
