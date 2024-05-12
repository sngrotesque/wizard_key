#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_SSL
#define WMKC_CPP_SSL
#include <network/socket.hpp>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/sslerr.h>
#include <openssl/crypto.h>

#ifdef WMKC_PLATFORM_WINOS
#if defined(_MSC_VER)
#pragma comment(lib, "libssl")
#pragma comment(lib, "libcrypto")
#endif
#endif

#define WMKC_SSL_STRICT

namespace wmkc {
    namespace net {
        class ssl_socket
        {
        public:
            SSL *ssl;
            net::Socket fd;

            ssl_socket(SSL *_ssl, net::Socket _fd);
        };

        class ssl_context
        {
        public:
            SSL_CTX *ssl_ctx;
            SSL *ssl;

            ssl_context(const SSL_METHOD *method);
            ~ssl_context();
            net::ssl_socket wrap_socket(net::Socket fd, std::string server_hostname = "");
        };
    }  // namespace net
}  // namespace wmkc

#endif /* WMKC_CPP_SSL */
#endif     /* WMKC_SUPPORT */