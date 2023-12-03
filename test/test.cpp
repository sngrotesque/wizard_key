#include <network/wmkc_net.hpp>
#include <network/wmkc_ssl.hpp>

#include <crypto/snc.hpp>

#include <wmkc_binascii.hpp>
#include <wmkc_base64.hpp>
#include <wmkc_random.hpp>
#include <wmkc_misc.hpp>
#include <wmkc_time.hpp>

using namespace std;
using namespace wmkcNet;

void timer(void (*func)())
{
    wmkcTime time;

    double start = time.time();
    func();
    double stop = time.time();

    cout << "time used: " << (stop-start) << endl;
}

int main()
{
#   ifdef WMKC_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    std::string target_host = "www.pixiv.net";
    wmkc_u16 target_port = 443;
    std::string sendbuf = (
        "GET / HTTP/1.1\r\n"
        "Host: " + target_host + "\r\n"
        "Accept: */*\r\n"
        "Connection: close\r\n"
        "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0\r\n\r\n");
    wmkcChar recvbuf[4096] = {0};

    wmkcSSL_Context *ssl = new wmkcSSL_Context(TLS_method());
    wmkcSSL_Socket ssl_socket = ssl->wrap_socket(Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), target_host);

    ssl_socket.fd.connect(target_host, target_port);
    SSL_connect(ssl_socket.ssl);

    SSL_write(ssl_socket.ssl, sendbuf.c_str(), sendbuf.size());
    SSL_read(ssl_socket.ssl, recvbuf, sizeof(recvbuf));

    cout << recvbuf << endl;

    ssl_socket.fd.close();

    delete ssl;
#   ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
