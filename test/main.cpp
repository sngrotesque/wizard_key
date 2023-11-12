// #include <cpp/includes/network/wmkc_net.hpp>
#include <cpp/includes/network/wmkc_ssl.hpp>

#include <cpp/includes/crypto/snc.hpp>
#include <cpp/includes/wmkc_base64.hpp>

#include <c/includes/wmkc_misc.h>

#define TARGET_ADDR "passport.bilibili.com"
#define TARGET_PORT 80

int main(int argc, char **argv)
{
#ifdef WMKC_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#endif

    wmkcSSL *ssl = new wmkcSSL();
    wmkcSSL_Socket sslSockfd;
    std::string sendbuf = (
        "GET /qrcode/getLoginUrl HTTP/1.1\r\n"
        "Host: passport.bilibili.com\r\n"
        "Accept: */*\r\n"
        "Connection: close\r\n"
        "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/116.0\r\n"
        "\r\n"
    );
    wmkcChar recvbuf[4096];

    sslSockfd = ssl->wrap_socket(new wmkcNet(AF_INET, SOCK_STREAM, IPPROTO_TCP), "passport.bilibili.com");
    sslSockfd.sockfd->connect("passport.bilibili.com", 443);
    SSL_connect(sslSockfd.ssl);

    SSL_write(sslSockfd.ssl, sendbuf.c_str(), sendbuf.size());
    SSL_read(sslSockfd.ssl, recvbuf, sizeof(recvbuf));

    std::cout << recvbuf << std::endl;

#ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#endif
    return 0;
}
