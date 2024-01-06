// #include <network/wmkc_net.hpp>
#include <network/wmkc_ssl.hpp>

#include <wmkc_misc.hpp>

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

            // cout << recvbuf << endl;
            wmkcMisc::PRINT_RAW((wmkcByte *)recvbuf, sizeof(recvbuf), 1);

            SSL_shutdown(ssl);
            SSL_CTX_free(ssl_ctx);
            SSL_free(ssl);
            sockfd->close();
            delete sockfd;
        }

        void http_test(std::string server_hostname, const uint16_t port, const std::string headers)
        {
            wmkcNet::Socket *sockfd = new wmkcNet::Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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

        void proxy_test(uint16_t proxy_port = 1080)
        {
            wmkcNet::Socket sockfd(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            string target_hostname = "127.0.0.1:9991";
            string http_headers = (
                "GET / HTTP/1.1\r\n"
                "Host: " + target_hostname + "\r\n"
                "Accept: text/html;q=0.8, image/png;q=0.9, */*\r\n"
                "Connection: close\r\n"
                "User-Agent: " + defaultUserAgent + "\r\n\r\n");

            sockfd.connect("127.0.0.1", proxy_port);
            sockfd.send("CONNECT " + target_hostname + " HTTP/1.1\r\nHost: " + target_hostname + "\r\n\r\n");
            cout << sockfd.recv(256) << endl;
            sockfd.send(http_headers);
            cout << sockfd.recv(4096) << endl;

            sockfd.close();
        }
};

int main(int argc, char **argv)
{
    net_test *net = new net_test();

    net->proxy_test();

    delete net;
    return 0;
}
