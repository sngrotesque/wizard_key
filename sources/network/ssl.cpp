#include <network/ssl.hpp>

wVoid wmkcSSL_exception(std::string funcName)
{
    wU32 err_code;
    wChar err_msg[256];
    err_code = ERR_get_error();
    ERR_error_string(err_code, err_msg);
    wmkc::exception(err_code, funcName, err_msg);
}

wmkc::net::ssl_socket::ssl_socket(SSL *_ssl, net::Socket _fd)
: ssl(_ssl), fd(_fd)
{

}

///////////////////////////////////////////////////////////////////////////////
wmkc::net::ssl_context::ssl_context(const SSL_METHOD *method)
: ssl_ctx(), ssl()
{
    // 如果用户未指定SSL版本
    if(!method) {
        method = TLS_method();
    }

    if(!(this->ssl_ctx = SSL_CTX_new(method))) {
        wmkcSSL_exception("wmkc::net::ssl_context::ssl_context");
    }

    // 指定SSL选项
    wSize options = (SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);
#   ifdef WMKC_SSL_STRICT
    options |= (SSL_OP_NO_TLSv1                 | SSL_OP_NO_TLSv1_1);
    options |= (SSL_OP_CIPHER_SERVER_PREFERENCE | SSL_OP_NO_RENEGOTIATION);
#   endif
#   ifdef SSL_OP_NO_COMPRESSION
    options |= SSL_OP_NO_COMPRESSION;
#   endif
#   ifdef SSL_OP_CIPHER_SERVER_PREFERENCE
    options |= SSL_OP_CIPHER_SERVER_PREFERENCE;
#   endif
#   ifdef SSL_OP_SINGLE_DH_USE
    options |= SSL_OP_SINGLE_DH_USE;
#   endif
#   ifdef SSL_OP_SINGLE_ECDH_USE
    options |= SSL_OP_SINGLE_ECDH_USE;
#   endif
    SSL_CTX_set_options(this->ssl_ctx, options);

    // 设置密码级别
    SSL_CTX_set_cipher_list(this->ssl_ctx, "HIGH:!aNULL:!eNULL");
    // 设置SSL协议最低版本
    SSL_CTX_set_min_proto_version(this->ssl_ctx, TLS1_2_VERSION);
    // 设置SSL协议模式
    SSL_CTX_set_mode(this->ssl_ctx, SSL_MODE_RELEASE_BUFFERS);
    // 设置SSL协议会话上下文ID
    SSL_CTX_set_session_id_context(this->ssl_ctx, (wByte *)"ssl_context", 15);

    // 根据CTX新建一个SSL对象
    if(!(this->ssl = SSL_new(this->ssl_ctx))) {
        SSL_CTX_free(this->ssl_ctx);
        wmkcSSL_exception("wmkc::net::ssl_context::ssl_context");
    }
}

wmkc::net::ssl_context::~ssl_context()
{
    SSL_free(this->ssl);
    SSL_CTX_free(this->ssl_ctx);
}

wmkc::net::ssl_socket wmkc::net::ssl_context::wrap_socket(wmkc::net::Socket fd,
                                                        std::string server_hostname)
{
    if(SSL_set_fd(this->ssl, fd.fd) != 1) {
        wmkcSSL_exception("wmkc::net::ssl_context::wrap_socket");
    }
    if(SSL_set_tlsext_host_name(this->ssl, server_hostname.c_str()) != 1) {
        wmkcSSL_exception("wmkc::net::ssl_context::wrap_socket");
    }
    return wmkc::net::ssl_socket(this->ssl, fd);
}
