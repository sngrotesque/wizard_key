#include <cjson/cJSON.h>

#include <cpp/includes/network/wmkc_ssl.hpp>
#include <library/qrcode/qrcode.hpp>

using namespace std;

namespace use_qrcode {
    using qrcodegen::QrCode;
    using qrcodegen::QrSegment;

    static void printQr(const QrCode &qr) {
        int border = 4;
        for (int y = -border; y < qr.getSize() + border; y++) {
            for (int x = -border; x < qr.getSize() + border; x++) {
                std::cout << (qr.getModule(x, y) ? "  " : "##");
                }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void qrcode_generator(const char *content)
    {
        printQr(QrCode::encodeText(content, QrCode::Ecc::LOW));
    }
}

namespace use_network {
    using namespace wmkcNet;

    Socket create_tcp_socket()
    {
        return Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }

    const std::string userAgent = "Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/116.0";

    void net_test(void (*func)(std::string, const uint16_t, const std::string), std::string addr, const uint16_t port, std::string headers)
    {
#       ifdef WMKC_PLATFORM_WINOS
        WSADATA ws;
        WSAStartup(MAKEWORD(2,2), &ws);
#       endif
        func(addr, port, headers);
#       ifdef WMKC_PLATFORM_WINOS
        WSACleanup();
#       endif
    }

    void https_test(std::string server_hostname, const uint16_t port, const std::string headers)
    {
        wmkcSSL_Context ssl = wmkcSSL_Context();
        wmkcSSL_Socket sockfd = ssl.wrap_socket(create_tcp_socket(), server_hostname);

        sockfd.connect(server_hostname, port);
        sockfd.send(headers);

        cout << sockfd.recv(4096) << endl;

        sockfd._fd.close();
    }

    void bilibili_login()
    {
        std::string qrcode_getLoginUrl_headers = (
            "GET /qrcode/getLoginUrl HTTP/1.1\r\n"
            "Host: passport.bilibili.com\r\n"
            "Connection: close\r\n"
            "User-Agent: " + userAgent + "\r\n\r\n");
        wmkcSSL_Socket sockfd = wmkcSSL_Context().wrap_socket(create_tcp_socket(), "passport.bilibili.com");
    }
}

int main(int argc, char **argv)
{
    use_network::bilibili_login();
    return 0;
}
