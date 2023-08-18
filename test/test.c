// #include <wmkc_hexdump.c>
// #include <wmkc_winapi.c>
// #include <wmkc_dict.c>
// #include <wmkc_sort.c>
// #include <crypto/snc_412.c>

// #include <network/wmkc_ddos.c>
#include <network/wmkc_chunk.c>
#include <network/wmkc_net.c>
// #include <crypto/wmkc_crypto.c>
#include <crypto/snc.c>
// #include <crypto/rsa.c>
// #include <crypto/pkc.c>
#include <wmkc_binascii.c>
#include <wmkc_memory.c>
#include <wmkc_common.c>
#include <wmkc_base64.c>
#include <wmkc_object.c>
#include <wmkc_random.c>
#include <wmkc_stream.c>
#include <wmkc_struct.c>
// #include <wmkc_thread.c>
#include <wmkc_basic.c>
#include <wmkc_coder.c>
// #include <wmkc_chat.c>
#include <wmkc_file.c>
#include <wmkc_hash.c>
#include <wmkc_time.c>
#include <wmkc_math.c>
#include <wmkc_misc.c>
#include <wmkc_img.c>
#include <wmkc_pad.c>

void win_net_init()
{
#   if defined(WMKC_PLATFORM_WINOS)
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif
}

void win_net_clear()
{
#   if defined(WMKC_PLATFORM_WINOS)
    WSACleanup();
#   endif
}

#define HOSTNAME "www.bilibili.com"
#define HOSTPORT 443
#define USERAGENT "Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0"

void HTTP_Client()
{
    win_net_init();

    wmkc_obj *SendStream = wmkcNull;
    wmkc_obj *RecvStream = wmkcNull;
    wmkcNet_obj *net = wmkcNull;
    wmkcNetBufT _recvbuf[4096];

    wmkcObj_new(&SendStream);
    wmkcObj_new(&RecvStream);
    wmkcNet_new(&net);

    printf("创建套接字。\n");
    wmkcNet_socket(net, AF_INET, SOCK_STREAM, 0);
    wmkcNet_settimeout(net, 0.5);
    printf("连接服务器：%s:%u\n", HOSTNAME, HOSTPORT);
    wmkcNet_connect(net, HOSTNAME, HOSTPORT);

    printf("构建发送流。\n");
    wmkcObj_append(SendStream, "GET / HTTP/1.1\r\n");
    wmkcObj_append(SendStream, "Acceot: text/html; image/jpeg; application/json; */*\r\n");
    wmkcObj_append(SendStream, "Connection: close\r\n");
    wmkcObj_append(SendStream, "Host: "HOSTNAME"\r\n");
    wmkcObj_append(SendStream, "User-Agent: "USERAGENT"\r\n\r\n");

    printf("发送发送流。\n");
    wmkcNet_sendall(net, SendStream->buf, SendStream->size, 0);

    printf("接收接收流。\n");
    for(;;) {
        wmkcMem_zero(_recvbuf, sizeof(_recvbuf));
        wmkcErr_obj error = wmkcNet_recv(net, _recvbuf, sizeof(_recvbuf), 0);
        if(error.code || !net->tSize) {
            printf("%s: %s\n", error.func, error.message);
            break;
        }
        wmkcObj_append(RecvStream, _recvbuf);
    }

    wmkcMisc_PRINT_RAW(RecvStream->buf, RecvStream->size, true);

    wmkcNet_shutdown(net, 2);
    wmkcNet_close(net);

    wmkcObj_free(&SendStream);
    wmkcObj_free(&RecvStream);
    wmkcNet_free(&net);

    win_net_clear();
}

WMKC_PRIVATE_CONST(wmkcByte) SNC_TEST_KEY[96] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
WMKC_PRIVATE_CONST(wmkcByte) SNC_TEST_IV[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define 🆒 wmkcByte
#define 🤝 malloc
#define 🚢 wmkcNull
#define 🔺 wmkcMem_zero
#define 👂 wmkcMem_free
#define 🍚 1024

void test()
{
    🆒 *😀 = 🚢;
    😀 = (🆒 *)🤝(🍚);

    🔺(😀, 🍚);
    memcpy(😀, "hello", 5);

    printf("%s\n", 😀);
    👂(😀);
}

int main(wmkc_s32 argc, wmkcChar **argv)
{
    void (*func)() = test;
    func();
    return 0;
}
