#include <network/snNet.h>
#include <network/snTransfer.h>
#include <errno.h>
#include <snHash.h>
#include <snTime.h>
#include <snMisc.h>

#include <snMisc.c>
#include <snHash.c>
#include <snTime.c>
#include <snFile.c>
#include <snObject.c>
#include <snBinascii.c>
#include <crypto/snc.c>
#include <network/snNet.c>
#include <network/snTransfer.c>

SN_PRIVATE(snVoid) snTransfer_listen_test()
{
    snTransfer_ctx *ctx = snNull;
    const snChar *addr = "0.0.0.0";
    snTransfer_new(&ctx, addr, SN_FT_DEFAULT_PORT, 5);

    snTransfer_Listen(ctx, "misc/snFT_listen.bin");

    snTransfer_release(&ctx);
}

SN_PRIVATE(snVoid) snTransfer_client_test()
{
    snTransfer_ctx *ctx = snNull;
    const snChar *addr = "47.108.209.65";
    // const snChar *addr = "47.243.162.23";
    // const snChar *addr = "127.0.0.1";

    snTransfer_new(&ctx, addr, SN_FT_DEFAULT_PORT, 5);

    snTransfer_Client(ctx, "misc/snFT.bin");

    snTransfer_release(&ctx);
}

SN_PRIVATE(snVoid) test()
{
    int code = 0;
    if(code == 1) {
        snTransfer_listen_test();
    } else {
        snTransfer_client_test();
    }
}

int main(int argc, char **argv)
{
    test();

    return 0;
}
