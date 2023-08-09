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
// #include <wmkc_object.c>
#include <wmkc_random.c>
// #include <wmkc_stream.c>
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

#if 1
static wmkcByte testKey[96] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f};
static wmkcByte testIv[32] = {
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f};
#endif

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

#define HOSTNAME "passport.bilibili.com"
#define HOSTPORT 443
#define HOSTUSER "Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0"

void net_test()
{
    wmkcNet_obj *net = wmkcNull;
    wmkcErr_obj error;

    wmkcNetBufT *content = (wmkcNetBufT *)(
        "GET /site/site.html HTTP/1.1\r\n"
        "Host: "HOSTNAME"\r\n"
        "Accept: */*\r\n"
        "Connection: keep-alive\r\n"
        "User-Agent: "HOSTUSER"\r\n\r\n"
    );
    wmkcNetBufT recvbuf[4096] = {0};

    wmkcNet_new(&net);
    wmkcNet_socket(net, AF_INET, SOCK_STREAM, IPPROTO_TCP);
    wmkcNet_settimeout(net, 5);
    wmkcNet_connect(net, HOSTNAME, HOSTPORT);
    wmkcNet_sendall(net, content, strlen(content), 0);
    wmkcNet_recv(net, recvbuf, sizeof(recvbuf), 0);
    wmkcNet_shutdown(net, 2);
    wmkcNet_close(net);
    wmkcNet_free(&net);

    printf("%s\n", recvbuf);
}

void set_timeout(wmkcNetSockT sockfd, int level, int optname, double _val)
{
#   if defined(WMKC_PLATFORM_WINOS)
    DWORD _timeout = (DWORD)(_val * 1000);
    wmkcChar *optval = (wmkcChar *)&_timeout;
#   elif defined(WMKC_PLATFORM_LINUX)
    double intpart = 0;
    double fracpart = modf(_val, &intpart);
    struct timeval _timeout = {.tv_sec=(long)intpart, .tv_usec=(long)(fracpart * 1000000)};
    wmkcVoid *optval = (wmkcVoid *)&_timeout;
#   endif
    setsockopt(sockfd, level, optname, optval, sizeof(_timeout));
}

void test()
{
    win_net_init();

    wmkcErr_obj error;
    wmkcNet_obj *net = wmkcNull;
    wmkcNet_obj *dst = wmkcNull;

    wmkcNet_new(&net);
    wmkcNet_socket(net, AF_INET, SOCK_STREAM, 0);
    error = wmkcNet_connect(net, "www.bilibili.com", 80);
    if(error.code) printf("%s: %s\n", error.func, error.message);

    wmkcNetBufT sendbuf[4096] = {
        "GET / HTTP/1.1\r\n"
        "Host: www.bilibili.com\r\n"
        "User-Agent: android\r\n\r\n"};
    wmkcNetBufT recvbuf[4096]= {0};

    wmkcNet_send(net, sendbuf, strlen(sendbuf), 0);
    wmkcNet_recv(net, recvbuf, sizeof(recvbuf), 0);

    printf("%s\n", recvbuf);

    wmkcNet_close(net);
    if(dst) wmkcNet_free(&dst);
    wmkcNet_free(&net);

    win_net_clear();
}

int main(wmkc_u32 argc, wmkcChar **argv)
{
    void (*func)() = test;
    func();
    return 0;
}
