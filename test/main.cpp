// #include <cpp/includes/network/wmkc_net.hpp>
#include <cpp/includes/network/wmkc_ssl.hpp>

using namespace std;

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

int main(int argc, char **argv)
{
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);

    string addr = "passport.bilibili.com";
    string headers = (
        "GET /qrcode/getLoginUrl HTTP/1.1\r\n"
        "Host: " + addr + "\r\n"
        "Accept: */*\r\n"
        "Connection: close\r\n"
        "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/116.0\r\n\r\n");

    https_test(addr, 443, headers);

    WSACleanup();
    return 0;
}
