#include <network/wmkc_net.h>
#include <wmkc_common.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>

#define HOSTNAME "passport.bilibili.com"
#define HOSTPORT 443
WMKC_PRIVATE_CONST(wmkcChar *) sendbuf = (
    "GET /qrcode/getLoginUrl HTTP/1.1\r\n"
    "Host: "HOSTNAME"\r\n"
    "Accept: */*\r\n"
    "User-Agent: Android\r\n\r\n"
);

void openssl_bio_test()
{
    SSL_CTX *ssl_ctx = wmkcNull;
    BIO *bio = wmkcNull;
    SSL *ssl = wmkcNull;
    wmkcNetBuf recvbuf[4096];

    ssl_ctx = SSL_CTX_new(TLS_client_method());
    bio = BIO_new_ssl_connect(ssl_ctx);

    BIO_get_ssl(bio, &ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    BIO_set_conn_hostname(bio, HOSTNAME":443");

    if (BIO_do_connect(bio) != 1) {
        printf("BIO_do_connect error.\n");
        return;
    }

    wmkcMem_secure(recvbuf, sizeof(recvbuf));
    BIO_write(bio, sendbuf, strlen(sendbuf));
    BIO_read(bio, recvbuf, sizeof(recvbuf));

    fwrite(recvbuf, 1, strlen(recvbuf), stdout);

    BIO_free_all(bio);
    SSL_CTX_free(ssl_ctx);
}

void openssl_socket_test()
{
    wmkcNet_obj *net = wmkcNull;
    SSL_CTX *ssl_ctx = wmkcNull;
    SSL *ssl = wmkcNull;
    wmkcNetBuf recvbuf[4096];

    wmkcNet_new(&net, wmkcNull, AF_INET, false);
    wmkcNet_init(net, HOSTNAME, HOSTPORT);
    wmkcNet_connect(net);
    ssl_ctx = SSL_CTX_new(TLS_client_method());
    ssl = SSL_new(ssl_ctx);

    SSL_set_fd(ssl, net->sockfd);
    SSL_connect(ssl);

    wmkcMem_secure(recvbuf, sizeof(recvbuf));
    SSL_write(ssl, sendbuf, strlen(sendbuf));
    SSL_read(ssl, recvbuf, sizeof(recvbuf));

    fwrite(recvbuf, 1, strlen(recvbuf), stdout);

    SSL_CTX_free(ssl_ctx);
    SSL_free(ssl);
    wmkcNet_close(net);
    wmkcNet_free(&net);
}