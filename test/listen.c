#include <network/wmkc_chunk.c>
#include <network/wmkc_net.c>
#include <wmkc_binascii.c>
#include <wmkc_memory.c>
#include <wmkc_common.c>
#include <wmkc_struct.c>
#include <wmkc_basic.c>
#include <wmkc_coder.c>
#include <wmkc_misc.c>

void listen_test(wmkcCSTR host, wmkc_u16 port)
{
    wmkcNet_obj *net = wmkcNull;
    wmkcNet_new(&net);
    wmkcNet_socket(net, AF_INET, SOCK_STREAM, 0);

    printf("绑定至：%s:%d\n", host, port);
    wmkcNet_bind(net, host, port);
    wmkcNet_listen(net, 3);

    wmkcNet_obj *clientNet = wmkcNull;
    wmkcNet_new(&clientNet);
    wmkcNet_socket(clientNet, AF_INET, SOCK_STREAM, 0);
    printf("等待连接...\n");
    wmkcNet_accept(clientNet, net);

    printf("等待接收...\n");
    wmkcNetBufT *content = wmkcNull;
    socklen_t size = 0;
    wmkcChunk_recv(clientNet, &content, &size);
    printf("content: "); wmkcMisc_PRINT_RAW(content, size, 1);
    printf("size: %d\n", size);
    wmkcMem_free(content);

    wmkcNet_shutdown(clientNet, 2);
    wmkcNet_close(clientNet);
    wmkcNet_free(&clientNet);
    wmkcNet_shutdown(net, 2);
    wmkcNet_close(net);
    wmkcNet_free(&net);
}

int main(int argc, char **argv)
{
#   ifdef WMKC_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif
    listen_test("0.0.0.0", 49281);
#   ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
