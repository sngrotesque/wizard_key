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
    snTime_ctx *timer = snNull;
    snTransfer_ctx *ctx = snNull;
    const snChar *addr = "0.0.0.0";
    const snChar *fn = "misc/00000001.mp4";

    timer = (snTime_ctx *)malloc(sizeof(snTime_ctx));
    snTransfer_new(&ctx, addr, SN_FT_DEFAULT_PORT, 5);

    snTime_TimerBegin(timer);
    snTransfer_Listen(ctx, fn);
    snTime_TimerEnd(timer);
    snTime_TimerPrint("Timer: ", timer);

    snTransfer_release(&ctx);
    free(timer);
}

SN_PRIVATE(snVoid) snTransfer_client_test()
{
    snTransfer_ctx *ctx = snNull;
    const snChar *addr = "47.108.209.65";
    // const snChar *addr = "47.243.162.23";
    // const snChar *addr = "127.0.0.1";

    snTransfer_new(&ctx, addr, SN_FT_DEFAULT_PORT, 5);

    snTransfer_Client(ctx, "p:/00000001.mp4");

    snTransfer_release(&ctx);
}

SN_PRIVATE(snVoid) snTransfer_test()
{
    int code = 0;
    if(code == 1) {
        snTransfer_listen_test();
    } else {
        snTransfer_client_test();
    }
}

SN_PRIVATE(snVoid) snNet_listen_test()
{
    snNet_ctx *net = snNull;
    snNetSize tSize;
    snByte buf[33];

    snZeroObject(buf, 33);

    printf("Initialize program...\n");
    snNet_new(&net, AF_INET);
    snNet_init(net, "0.0.0.0", SN_FT_DEFAULT_PORT, false);

    printf("Set timeout to 15 seconds.\n");
    snNet_timeout(net, 15);

    printf("socket bind...\n");
    snNet_bind(net, 1);
    printf("socket listen...\n");
    snNet_listen(net, 5);
    printf("Waiting to be connected...\n");
    snNet_accept(net);

    SOCKADDR_IN *ipv4 = net->info->client;
    printf("Client info: %s:%d\n",
        snNet_GetAddr(ipv4->sin_addr), snNet_GetPort(ipv4->sin_port));

    snNet_recv(net, &tSize, buf, 32);
    snNet_send(net, &tSize, "This machine received a message.", 32);

    printf("Message from the sending end: %s\n", buf);

    snNet_close(net);
    // snNet_release(&net);
}

SN_PRIVATE(snVoid) snNet_client_test()
{
    snNet_ctx *net = snNull;
    snNetSize tSize;
    snByte buf[33];

    snZeroObject(buf, 33);

    printf("Initialize program...\n");
    snNet_new(&net, AF_INET);
    snNet_init(net, "47.108.209.65", SN_FT_DEFAULT_PORT, false);

    printf("Set timeout to 15 seconds.\n");
    snNet_timeout(net, 15);

    SOCKADDR_IN *ipv4 = net->info->receiver;
    printf("Server info: %s:%d\n",
        snNet_GetAddr(ipv4->sin_addr), snNet_GetPort(ipv4->sin_port));
    snNet_connect(net);

    snNet_send(net, &tSize, "0123456789abcdef0123456789abcdef", 32);
    snNet_recv(net, &tSize, buf, 32);

    printf("Message from the receiving end: %s\n", buf);

    snNet_close(net);
    // snNet_release(&net);
}

int main(int argc, char **argv)
{
    // snNet_listen_test();
    snNet_client_test();

    return 0;
}
