// #include <cpp/includes/network/wmkc_net.hpp>
#include <cpp/includes/network/wmkc_ssl.hpp>

#include <cpp/includes/crypto/snc.hpp>
#include <cpp/includes/wmkc_base64.hpp>

#include <c/includes/wmkc_misc.h>

#define TARGET_ADDR "passport.bilibili.com"
#define TARGET_PORT 443
#define HEADERS ("GET /qrcode/getLoginUrl HTTP/1.1\r\nHost:" TARGET_ADDR "\r\n"\
    "User-Agent:Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/116.0\r\n\r\n")

using namespace std;
using namespace wmkcNet;

int main(int argc, char **argv)
{
#ifdef WMKC_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#endif
    wmkcSSL_Context *context = new wmkcSSL_Context(TLS_method());
    wmkcSSL_Socket sockfd = context->wrap_socket(new wmkcNet::Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), TARGET_ADDR);
    wmkcChar recvbuf[4096];

    sockfd._fd->connect(TARGET_ADDR, TARGET_PORT);
    SSL_connect(sockfd.ssl);

    SSL_write(sockfd.ssl, HEADERS, strlen(HEADERS));
    SSL_read(sockfd.ssl, recvbuf, 4096);
    sockfd._fd->close();

    cout << recvbuf << endl;
    delete context;
#ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#endif
    return 0;
}
