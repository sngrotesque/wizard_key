#include <network/wmkc_ssl.c>
#include <network/wmkc_net.c>

#define HOSTNAME "www.pixiv.net"
#define SSL_DISABLE(_Ctx) SSL_CTX_set_options(_Ctx, \
    SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | \
    SSL_OP_NO_TLSv1 | SSL_OP_NO_TLSv1_1 | \
    SSL_OP_CIPHER_SERVER_PREFERENCE | SSL_OP_NO_RENEGOTIATION);\
    SSL_CTX_set_cipher_list(_Ctx, "HIGH:!aNULL:!eNULL");

static const char* sendbuf = (
    "GET / HTTP/1.1\r\n"
    "Host: "HOSTNAME"\r\n"
    "Accept: */*\r\n"
    "Connection: close\r\n"
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0\r\n\r\n"
);

SSL_CTX *ssl_ctx = NULL;
BIO *bio = NULL;
SSL *ssl = NULL;
char recvbuf[4096];
wmkcErr_obj error;

void openssl_sock_test()
{
    ssl_ctx = SSL_CTX_new(TLS_method());
    SSL_DISABLE(ssl_ctx);
    ssl = SSL_new(ssl_ctx);

    wmkcNet_obj *net = wmkcNull;
    wmkcNet_new(&net);
    if((error = wmkcNet_socket(net, AF_INET, SOCK_STREAM, 0)).code)
        printf("%s: %s\n", error.func, error.message);

    if(!SSL_set_fd(ssl, net->sockfd)) {
        ERR_print_errors_fp(stderr);
        return;
    }
    if(!SSL_set_tlsext_host_name(ssl, HOSTNAME)) {
        ERR_print_errors_fp(stderr);
        return;
    }

    if((error = wmkcNet_connect(net, "104.18.26.235", 443)).code)
        printf("%s: %s\n", error.func, error.message);
    if(!SSL_connect(ssl)) {
        ERR_print_errors_fp(stderr);
        return;
    }

    memset(recvbuf, 0, sizeof(recvbuf));
    SSL_write(ssl, sendbuf, strlen(sendbuf));
    SSL_read(ssl, recvbuf, sizeof(recvbuf));

    fwrite(recvbuf, 1, strlen(recvbuf), stdout);

    wmkcNet_shutdown(net, 2);
    wmkcNet_close(net);
    wmkcNet_free(&net);
    SSL_CTX_free(ssl_ctx);
    SSL_free(ssl);
}

void openssl_bio_test()
{
    ssl_ctx = SSL_CTX_new(TLS_method());
    SSL_DISABLE(ssl_ctx);
    bio = BIO_new_ssl_connect(ssl_ctx);

    BIO_get_ssl(bio, &ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    BIO_set_conn_hostname(bio, HOSTNAME);
    BIO_set_conn_port(bio, "443");

    if (BIO_do_connect(bio) != 1) {
        ERR_print_errors_fp(stderr);
        return;
    }

    memset(recvbuf, 0, sizeof(recvbuf));
    BIO_write(bio, sendbuf, strlen(sendbuf));
    BIO_read(bio, recvbuf, sizeof(recvbuf));

    fwrite(recvbuf, 1, strlen(recvbuf), stdout);

    BIO_free_all(bio);
    SSL_CTX_free(ssl_ctx);
}

int main(int argc, char **argv)
{
#   ifdef WMKC_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    printf("Action...\n");
    openssl_sock_test();
    // openssl_bio_test();

#   ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
