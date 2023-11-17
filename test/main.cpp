// #include <cpp/includes/network/wmkc_net.hpp>
#include <cpp/includes/network/wmkc_ssl.hpp>

using namespace std;
static const std::string defaultUserAgent = "Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0";

class net_test {
    public:
        net_test()
        {
#           ifdef WMKC_PLATFORM_WINOS
            WSADATA ws;
            WSAStartup(MAKEWORD(2,2), &ws);
#           endif
        }

        ~net_test()
        {
#           ifdef WMKC_PLATFORM_WINOS
            WSACleanup();
#           endif
        }

        void https_test(std::string server_hostname, const uint16_t port, const std::string headers, const SSL_METHOD *protocol = TLS_method())
        {
            wmkcNet::Socket *sockfd = wmkcNull;
            SSL_CTX *ssl_ctx = wmkcNull;
            SSL *ssl = wmkcNull;
            wmkcChar recvbuf[4096] = {0};

            sockfd = new wmkcNet::Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            ssl_ctx = SSL_CTX_new(protocol);
            SSL_CTX_set_options(ssl_ctx, (SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_TLSv1 | SSL_OP_NO_TLSv1_1 |
                SSL_OP_CIPHER_SERVER_PREFERENCE | SSL_OP_NO_RENEGOTIATION | SSL_OP_NO_COMPRESSION |
                SSL_OP_CIPHER_SERVER_PREFERENCE | SSL_OP_SINGLE_DH_USE | SSL_OP_SINGLE_ECDH_USE));
            SSL_CTX_set_cipher_list(ssl_ctx, "HIGH:!aNULL:!eNULL");
            SSL_CTX_set_min_proto_version(ssl_ctx, TLS1_2_VERSION);
            SSL_CTX_set_mode(ssl_ctx, SSL_MODE_RELEASE_BUFFERS);
            SSL_CTX_set_session_id_context(ssl_ctx, (wmkcByte *)"wmkcSSL_Context", 15);
            ssl = SSL_new(ssl_ctx);
            SSL_set_fd(ssl, sockfd->fd);
            SSL_set_tlsext_host_name(ssl, server_hostname.c_str());

            sockfd->connect(server_hostname, port);
            SSL_connect(ssl);

            SSL_write(ssl, headers.c_str(), headers.size());
            SSL_read(ssl, recvbuf, sizeof(recvbuf));

            cout << recvbuf << endl;

            SSL_shutdown(ssl);
            SSL_CTX_free(ssl_ctx);
            SSL_free(ssl);
            sockfd->close();
            delete sockfd;
        }

        void http_test(std::string server_hostname, const uint16_t port, const std::string headers)
        {
            wmkcNet::Socket *sockfd = wmkcNull;

            sockfd->connect(server_hostname, port);
            sockfd->sendall(headers);

            cout << sockfd->recv(4096) << endl;

            sockfd->shutdown(2);
            sockfd->close();

            delete sockfd;
        }

        void wmkcNet_Socket_test()
        {
            wmkcNet::Socket *sockfd = new wmkcNet::Socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);

            sockfd->connect("www.bilibili.com", 80);
            sockfd->send("GET / HTTP/1.1\r\nHost: www.bilibili.com\r\nUser-Agent: Android\r\n\r\n");
            cout << sockfd->recv(4096) << endl;
            sockfd->close();

            delete sockfd;
        }
};

int main(int argc, char **argv)
{
    net_test *net = new net_test();

    string headers = \
        "POST /qrcode/getLoginInfo HTTP/1.1\r\n"
        "Host: passport.bilibili.com\r\n"
        "Accept: text/html;q=0.9, application/json;q=0.8, */*\r\n"
        "Connection: close\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: 41\r\n"
        "DNT: 1\r\n"
        "User-Agent: " + defaultUserAgent + "\r\n"
        "\r\n"
        "oauthKey=e6d9679f1fd8acb6ea88b53a27fb2d2b";

    net->https_test("passport.bilibili.com", 443, headers);

    delete net;
    return 0;
}
