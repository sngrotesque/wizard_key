#include <config/wuk.hpp>

#if WUK_SUPPORT
#ifndef WUK_CPP_SSL
#define WUK_CPP_SSL
#include <network/socket.hpp>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/sslerr.h>
#include <openssl/crypto.h>

#if defined(WUK_PLATFORM_WINOS) && defined(_MSC_VER)
#   pragma comment(lib, "libssl")
#   pragma comment(lib, "libcrypto")
#endif

#define WUK_SSL_STRICT

namespace wuk {
    namespace net {
        class LIBWUK_API SSL_Socket {
        protected:
            SSL *ssl;
            net::Socket fd;
            wS32 transmissionLength; // 单次传输长度
        public:
            // SSL_Socket();
            SSL_Socket(SSL *_ssl, wuk::net::Socket _fd);

            void connect(const std::string addr, const wU16 port);
            void send(const std::string content);
            std::string recv(const wS32 length);
        };

        class LIBWUK_API SSL_Context {
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

#endif /* WUK_CPP_SSL */
#endif /* WUK_SUPPORT */