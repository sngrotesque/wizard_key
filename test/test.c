
// #include <wmkc_winapi.c>
// #include <wmkc_dict.c>
// #include <wmkc_sort.c>
// #include <crypto/snc_412.c>

#include <network/wmkc_chunk.c>
#include <network/wmkc_net.c>
#include <network/wmkc_ssl.c>
#include <crypto/snc.c>
#include <wmkc_binascii.c>
#include <wmkc_hexdump.c>
#include <wmkc_memory.c>
#include <wmkc_common.c>
#include <wmkc_base64.c>
#include <wmkc_object.c>
#include <wmkc_random.c>
#include <wmkc_stream.c>
#include <wmkc_struct.c>
#include <wmkc_basic.c>
#include <wmkc_coder.c>
#include <wmkc_file.c>
#include <wmkc_hash.c>
#include <wmkc_time.c>
#include <wmkc_math.c>
#include <wmkc_misc.c>
#include <wmkc_img.c>
#include <wmkc_png.c>
#include <wmkc_pad.c>

#if 1
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

#define HOSTNAME "music.163.com"
#define HOSTPORT 443
#define USERAGENT "Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0"

void HTTP_Client()
{
    win_net_init();
    wmkcSSL_obj *ssl_ctx = wmkcNull;
    wmkcNet_obj *sockfd = wmkcNull;
    wmkc_obj *SendStream = wmkcNull;
    wmkc_obj *RecvStream = wmkcNull;
    wmkcByte recvbuf[4096];
    wmkcErr_obj error;

    wmkcSSL_new(&ssl_ctx);
    wmkcNet_new(&sockfd);

    wmkcObj_new(&SendStream);
    wmkcObj_new(&RecvStream);
    wmkcSSL_context(ssl_ctx, TLS_method());
    wmkcNet_socket(sockfd, AF_INET, SOCK_STREAM, IPPROTO_TCP);
    wmkcNet_settimeout(sockfd, 2.0);
    wmkcSSL_wrap_socket(ssl_ctx, sockfd, HOSTNAME);
    error = wmkcSSL_connect(ssl_ctx, HOSTNAME, HOSTPORT);
    if(error.code) printf("%s: %s\n", error.func, error.message);

    wmkcObj_append(SendStream, "GET / HTTP/1.1\r\n");
    wmkcObj_append(SendStream, "Host: "HOSTNAME"\r\n");
    wmkcObj_append(SendStream, "Accept: */*; text/html\r\n");
    wmkcObj_append(SendStream, "Accept-Encoding: identity\r\n");
    wmkcObj_append(SendStream, "Connection: close\r\n");
    wmkcObj_append(SendStream, "User-Agent: "USERAGENT"\r\n\r\n");
    wmkcSSL_sendall(ssl_ctx, (wmkcNetBufT *)SendStream->buf, SendStream->size);

    for(;;) {
        wmkcMem_secure(recvbuf, sizeof(recvbuf));
        if((error = wmkcSSL_recv(ssl_ctx, (wmkcNetBufT *)recvbuf, sizeof(recvbuf)-1)).code) {
            break;
        }

        wmkcObj_append(RecvStream, (wmkcCSTR)recvbuf);
    }

    wmkcFile_fwrite(RecvStream->buf, RecvStream->size, "test."HOSTNAME".html");
    wmkcMisc_PRINT_RAW(RecvStream->buf, RecvStream->size, 1);

    wmkcNet_close(sockfd);
    wmkcSSL_free(&ssl_ctx);
    wmkcObj_free(&SendStream);
    wmkcObj_free(&RecvStream);
    win_net_clear();
}

void test()
{
    wmkcErr_obj error;
    wmkcPNG_obj *png = wmkcNull;
    wmkcPNG_new(&png);

    error = wmkcPNG_read(png, 0, "I:/Pitchers/VisualStudio2022_profile_1560x1560.png");
    if(error.code) {
        printf("%s[%d]: %s\n", error.func, (wmkc_s32)error.code, error.message);
        return;
    }

    printf("PNG Width:      %u\n",   png->width);
    printf("PNG height:     %u\n",   png->height);
    printf("PNG Bit depth:  %02x\n", png->bitDepth);
    printf("PNG Color type: %02x\n", png->colorType);
    printf("PNG Compress:   %02x\n", png->compressMethod);
    printf("PNG Filter:     %02x\n", png->filterMethod);
    printf("PNG Interlace:  %02x\n", png->interlaceMethod);

    wmkcPNG_free(&png);
}

int main(wmkc_s32 argc, wmkcChar **argv)
{
    void (*func)() = test;
    func();
    return 0;
}
