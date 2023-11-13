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

namespace wmkcNet {
    class wmkcSSL_Socket {
        public:
            wmkcNet::Socket *_fd;
            SSL *ssl;

            wmkcSSL_Socket(wmkcNet::Socket *_sock, SSL *_ssl)
            : _fd(_sock), ssl(_ssl)
            {

            }
    };

    class wmkcSSL_Context {
        private:
            SSL_CTX *ssl_ctx;
            SSL *ssl;
            const SSL_METHOD *protocol;

        public:
            wmkcSSL_Context(const SSL_METHOD *protocol = TLS_method())
            : protocol(protocol)
            {
                wmkcSize options = SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3;
                /* strict mode: begin */
                options |= (SSL_OP_NO_TLSv1                 | SSL_OP_NO_TLSv1_1);
                options |= (SSL_OP_CIPHER_SERVER_PREFERENCE | SSL_OP_NO_RENEGOTIATION);
                /* strict mode: end */
#               ifdef SSL_OP_NO_COMPRESSION
                options |= SSL_OP_NO_COMPRESSION;
#               endif
#               ifdef SSL_OP_CIPHER_SERVER_PREFERENCE
                options |= SSL_OP_CIPHER_SERVER_PREFERENCE;
#               endif
#               ifdef SSL_OP_SINGLE_DH_USE
                options |= SSL_OP_SINGLE_DH_USE;
#               endif
#               ifdef SSL_OP_SINGLE_ECDH_USE
                options |= SSL_OP_SINGLE_ECDH_USE;
#               endif
                if(!(this->ssl_ctx = SSL_CTX_new(this->protocol))) {
                    throw wmkcNet::wmkcNet_exception(wmkcErr_Err32, "wmkcSSL::wmkcSSL",
                        "Failed to establish SSL context.");
                }

                SSL_CTX_set_options(this->ssl_ctx, options);
                SSL_CTX_set_cipher_list(this->ssl_ctx, "HIGH:!aNULL:!eNULL");
                SSL_CTX_set_min_proto_version(this->ssl_ctx, TLS1_2_VERSION);
                SSL_CTX_set_mode(this->ssl_ctx, SSL_MODE_RELEASE_BUFFERS);

                const wmkcChar *ssl_ctx_sid_ctx = "wmkcSSL_Context";
                SSL_CTX_set_session_id_context(this->ssl_ctx, (wmkcByte *)ssl_ctx_sid_ctx, strlen(ssl_ctx_sid_ctx));

                if(!(this->ssl = SSL_new(this->ssl_ctx))) {
                    throw wmkcNet::wmkcNet_exception(wmkcErr_Err32, "wmkcSSL::wmkcSSL",
                        "Failed to create a new SSL object.");
                }
            }

            ~wmkcSSL_Context()
            {
                SSL_CTX_free(this->ssl_ctx);
                SSL_free(this->ssl);
            }

            // 去你妈C++，去你妈的OpenSSL，操你妈
            wmkcSSL_Socket wrap_socket(wmkcNet::Socket *sock, std::string server_hostname = "")
            {
                wmkcSSL_Socket sslSocket(sock, this->ssl);
                SSL_set_fd(this->ssl, sock->fd);
                SSL_set_tlsext_host_name(this->ssl, server_hostname.c_str());
                return wmkcSSL_Socket(sock, this->ssl);
            }
    };
};

#endif /* WMKC_CPP_SSL */
#endif /* WMKC_SUPPORT */