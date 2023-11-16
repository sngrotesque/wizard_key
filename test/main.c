#include <wmkc_base64.h>

int main()
{
    wmkcByte *dst = wmkcNull;
    wmkcSize dstSize = 0;

    wmkcBase64_encode(&dst, &dstSize, "SN-Grotesque", 12);

    printf("%s\n", dst);

    return 0;
}