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
        class SSL_Socket {
        public:
            SSL *ssl;
            net::Socket fd;

            SSL_Socket(SSL *_ssl, wmkc::net::Socket _fd);
        };

        class SSL_Context {
        public:
            SSL_CTX *ssl_ctx;
            SSL *ssl;

            SSL_Context(const SSL_METHOD *method);
            ~SSL_Context();
            net::SSL_Socket wrap_socket(net::Socket fd, std::string server_hostname = "");
        };
    }  // namespace net
}  // namespace wmkc

#endif /* WMKC_CPP_SSL */
#endif     /* WMKC_SUPPORT */