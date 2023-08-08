#include <network/wmkc_chunk.c>
#include <network/wmkc_net.c>
#include <wmkc_binascii.c>
#include <wmkc_memory.c>
#include <wmkc_common.c>
#include <wmkc_struct.c>
#include <wmkc_basic.c>
#include <wmkc_coder.c>
#include <wmkc_misc.c>

void client_test(wmkcCSTR host, wmkc_u16 port)
{
    wmkcNet_obj *net = wmkcNull;
    wmkcNet_new(&net);
    wmkcNet_socket(net, AF_INET, SOCK_STREAM, 0);

    printf("连接至：%s:%d\n", host, port);
    wmkcNet_connect(net, host, port);

    wmkcNetBufT *content = (wmkcNetBufT *)(
        "POST /login HTTP/1.1\r\n"
        "Host: sngrotesque.com\r\n"
        "Accept: text/html; image/jpeg; image/png; */*\r\n"
        "Connection: keep-alive\r\n"
        "DNT: 1\r\n"
        "Referer: https://sngrotesque.com\r\n"
        "Origin: https://sngrotesque.com\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: 22\r\n"
        "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0\r\n\r\n"
        "user=admin&pass=123456");
    socklen_t size = strlen(content);
    wmkcChunk_send(net, content, size);

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
    client_test("127.0.0.1", 49281);
#   ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
