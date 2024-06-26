#include <network/ssl.hpp>

void wmkcSSL_exception(std::string funcName)
{
    wU32 err_code = ERR_get_error();
    char err_msg[256]{};
    ERR_error_string(err_code, err_msg);
    throw wmkc::Exception(err_code, funcName, err_msg);
}

wmkc::net::SSL_Socket::SSL_Socket(SSL *_ssl, wmkc::net::Socket _fd)
: ssl(_ssl), fd(_fd), transmissionLength()
{

}

void wmkc::net::SSL_Socket::connect(const std::string addr, const wU16 port)
{
    try {
        this->fd.connect(addr, port);
        if(SSL_connect(this->ssl) != 1) {
            wmkcSSL_exception("wmkc::net::SSL_Socket::connect");
        }
    } catch (std::exception &e) {
        throw;
    }
}

void wmkc::net::SSL_Socket::send(const std::string content)
{
    const char *buffer = content.c_str();
    wSize length = content.size();

    this->transmissionLength = SSL_write(this->ssl, buffer, length);
    if(this->transmissionLength <= 0) {
        wmkcSSL_exception("wmkc::net::SSL_Socket::send");
    }
}

std::string wmkc::net::SSL_Socket::recv(const wS32 length)
{
    char *buffer = new (std::nothrow) char[length];
    if(!buffer) {
        throw wmkc::Exception(wmkcErr_ErrMemory, "wmkc::net::SSL_Socket::recv",
            "Failed to allocate memory for buffer.");
    }

    this->transmissionLength = SSL_read(this->ssl, buffer, length);
    if(this->transmissionLength <= 0) {
        wmkcSSL_exception("wmkc::net::SSL_Socket::recv");
    }

    std::string result{buffer, (wSize)this->transmissionLength};
    delete[] buffer;
    return result;
}

///////////////////////////////////////////////////////////////////////////////
wmkc::net::SSL_Context::SSL_Context(const SSL_METHOD *method)
: ssl_ctx(), ssl()
{
    if(!method) {
        method = TLS_method();
    }
    if(!(this->ssl_ctx = SSL_CTX_new(method))) {
        wmkcSSL_exception("wmkc::net::SSL_Context::SSL_Context");
    }
    const wByte *ssl_sid = reinterpret_cast<const wByte*>("wmkcSSL_Context");
    wU32 ssl_sid_len = static_cast<wU32>(strlen(
                        reinterpret_cast<const char *>(ssl_sid)));

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
    SSL_CTX_set_session_id_context(this->ssl_ctx, ssl_sid, ssl_sid_len);

    // 根据CTX新建一个SSL对象
    if(!(this->ssl = SSL_new(this->ssl_ctx))) {
        SSL_CTX_free(this->ssl_ctx);
        wmkcSSL_exception("wmkc::net::SSL_Context::SSL_Context");
    }
}

wmkc::net::SSL_Context::~SSL_Context()
{
}

wmkc::net::SSL_Socket wmkc::net::SSL_Context::wrap_socket(wmkc::net::Socket fd,
                                                        std::string server_hostname)
{
    if(SSL_set_fd(this->ssl, fd.fd) != 1) {
        wmkcSSL_exception("wmkc::net::SSL_Context::wrap_socket");
    }
    if(SSL_set_tlsext_host_name(this->ssl, server_hostname.c_str()) != 1) {
        wmkcSSL_exception("wmkc::net::SSL_Context::wrap_socket");
    }
    return wmkc::net::SSL_Socket(this->ssl, fd);
}

void wmkc::net::SSL_Context::destroy()
{
    SSL_free(this->ssl);
    SSL_CTX_free(this->ssl_ctx);
}
