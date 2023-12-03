#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_SSL
#define WMKC_CPP_SSL
#include <wmkc_exception.hpp>
#include <network/wmkc_net.hpp>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/sslerr.h>
#include <openssl/crypto.h>

#define WMKC_SSL_STRICT

namespace wmkcNet {
    class wmkcSSL_Socket {
        public:
            SSL *ssl;
            wmkcNet::Socket fd;

            wmkcSSL_Socket(SSL *_ssl, wmkcNet::Socket _fd);
    };

    class wmkcSSL_Context {
        public:
            SSL_CTX *ssl_ctx;
            SSL *ssl;

            wmkcSSL_Context(const SSL_METHOD *method);
            ~wmkcSSL_Context();
            wmkcNet::wmkcSSL_Socket wrap_socket(wmkcNet::Socket fd, std::string server_hostname = "");
    };
}

#endif /* WMKC_CPP_SSL */
#endif /* WMKC_SUPPORT */