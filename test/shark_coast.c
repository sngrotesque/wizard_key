#include <crypto/snc.h>

int main()
{
    SNC_ctx *snc = snNull;

    static snByte buf[32] = {1};
    static snByte key[32];
    static snByte iv[32];

    SNC_new(&snc, SNC_256);
    SNC_init(snc, key, iv);

    SNC_CBC_Encrypt(snc, buf, 32);

    for(int x = 0; x < 32; ++x) {
        printf("%02x", buf[x]);
    }
    printf("\n");

    SNC_release(&snc);
}
