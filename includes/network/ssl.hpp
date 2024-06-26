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

#if defined(WMKC_PLATFORM_WINOS) && defined(_MSC_VER)
#   pragma comment(lib, "libssl")
#   pragma comment(lib, "libcrypto")
#endif

#define WMKC_SSL_STRICT

namespace wmkc {
    namespace net {
        class LIBWMKC_API SSL_Socket {
        protected:
            SSL *ssl;
            net::Socket fd;
            wS32 transmissionLength; // 单次传输长度
        public:
            // SSL_Socket();
            SSL_Socket(SSL *_ssl, wmkc::net::Socket _fd);

            void connect(const std::string addr, const wU16 port);
            void send(const std::string content);
            std::string recv(const wS32 length);
        };

        class LIBWMKC_API SSL_Context {
        protected:
            SSL_CTX *ssl_ctx;
            SSL *ssl;
        public:
            SSL_Context(const SSL_METHOD *method = nullptr);
            ~SSL_Context();
            net::SSL_Socket wrap_socket(net::Socket fd, std::string server_hostname = "");
            void destroy();
        };
    }
}

#endif /* WMKC_CPP_SSL */
#endif /* WMKC_SUPPORT */