// #include <wmkc_hexdump.c>
// #include <wmkc_winapi.c>
// #include <wmkc_dict.c>
// #include <wmkc_sort.c>
// #include <crypto/snc_412.c>

// #include <network/wmkc_ddos.c>
// #include <network/wmkc_chunk.c>
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
    0xbc, 0x65, 0x5c, 0x7b, 0x34, 0x78, 0x74, 0x6e, 0x49, 0x62, 0x6d, 0x3b, 0x7a, 0x77, 0x5d, 0x53,
    0x71, 0x52, 0x4a, 0x30, 0x3e, 0x2e, 0x63, 0x24, 0x67, 0x3b, 0x6e, 0x61, 0x6b, 0x3a, 0x21, 0x73,
    0x30, 0x66, 0x6c, 0x23, 0x74, 0x76, 0x45, 0x5d, 0x22, 0x3d, 0x50, 0x79, 0x60, 0x6c, 0x5d, 0x75,
    0x70, 0x6a, 0x68, 0x66, 0x52, 0x6e, 0x55, 0x4c, 0x7c, 0x51, 0x4e, 0x28, 0x31, 0x72, 0x7a, 0x40,
    0x49, 0x4e, 0x3b, 0x36, 0x6b, 0x69, 0x54, 0x37, 0x35, 0x70, 0x30, 0x3e, 0x3d, 0x60, 0x73, 0x42,
    0x5f, 0x61, 0x6f, 0x79, 0x7a, 0x36, 0x5e, 0x6d, 0x35, 0x40, 0x5f, 0x6d, 0x63, 0x57, 0x49, 0x31};
static wmkcByte testIv[32] = {
    0x91, 0x75, 0x76, 0x36, 0x23, 0x59, 0x72, 0x29, 0x6e, 0x40, 0x5f, 0x65, 0x2d, 0x55, 0x75, 0x68,
    0x3d, 0x41, 0x78, 0x36, 0x4c, 0x50, 0x7d, 0x73, 0x61, 0x4e, 0x33, 0x6f, 0x23, 0x47, 0x4c, 0x36};
#endif

#define HOSTNAME "127.0.0.1"
#define HOSTPORT 49281
#define HOSTUSER "Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0"

void net_test()
{
#   if defined(WMKC_PLATFORM_WINOS)
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif
    wmkcNet_obj *net = wmkcNull;
    wmkcErr_obj error;

    wmkcNetBufT *content = (wmkcNetBufT *)(
        "GET / HTTP/1.1\r\n"
        "Host: www.baidu.com\r\n"
        "Accept: */*\r\n"
        "Accept-Encoding: br\r\n"
        "Connection: keep-alive\r\n"
        "User-Agent: "HOSTUSER"\r\n\r\n"
    );

    wmkcNet_new(&net);
    wmkcNet_socket(net, AF_INET, SOCK_STREAM, IPPROTO_TCP);
    wmkcNet_settimeout(net, 5);
    wmkcNet_connect(net, HOSTNAME, HOSTPORT);
    printf("Waiting...\n");
    wmkcTime_sleep(10.0);

    error = wmkcNet_sendall(net, content, strlen(content), 0);
    printf("[%d] %s: %s\n", (int)error.code, error.func, error.message);
    wmkcNet_shutdown(net, 2);
    wmkcNet_close(net);
    wmkcNet_free(&net);

#   if defined(WMKC_PLATFORM_WINOS)
    WSACleanup();
#   endif
}

void test()
{
    net_test();
}

int main(wmkc_u32 argc, wmkcChar **argv)
{
    void (*func)() = test;
    func();
    return 0;
}
