#include <cpp/includes/network/wmkc_net.hpp>

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

    wmkcNet *sockfd = new wmkcNet(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    std::string headers = (
        "GET /qrcode/getLoginUrl HTTP/1.1\r\n"
        "Host: " TARGET_ADDR "\r\n"
        "Accept: */*\r\n"
        "Connection: close\r\n"
        "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/116.0\r\n"
        "\r\n");

    sockfd->connect(TARGET_ADDR, TARGET_PORT);
    sockfd->sendall(headers);
    std::cout << sockfd->recv(4096) << std::endl;
    sockfd->close();

    delete sockfd;

#ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#endif
    return 0;
}
