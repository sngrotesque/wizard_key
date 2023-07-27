#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/sslerr.h>
#include <openssl/crypto.h>

#if defined(__linux)
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#elif defined(_WIN32)
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")
#endif

#define HOSTNAME "www.pixiv.net"
static const char* sendbuf = (
    "GET / HTTP/1.1\r\n"
    "Host: "HOSTNAME"\r\n"
    "Accept: */*\r\n"
    "Connection: keep-alive\r\n"
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0\r\n\r\n"
);

void openssl_bio_test()
{
    SSL_CTX *ssl_ctx = NULL;
    BIO *bio = NULL;
    SSL *ssl = NULL;
    char recvbuf[4096];

    ssl_ctx = SSL_CTX_new(TLS_client_method());
    SSL_CTX_set_options(ssl_ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);
    bio = BIO_new_ssl_connect(ssl_ctx);

    BIO_get_ssl(bio, &ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    BIO_set_conn_hostname(bio, HOSTNAME":443");

    if (BIO_do_connect(bio) != 1) {
        printf("BIO_do_connect error.\n");
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
    openssl_bio_test();
    return 0;
}
