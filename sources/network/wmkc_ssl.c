#include <network/wmkc_ssl.h>

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_new WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcSSL_new", "obj is NULL.");
    }

    if(!wmkcMem_new(wmkcSSL_obj *, (*obj), sizeof(wmkcSSL_obj))) {
        wmkcErr_func_return(error, wmkcErr_ErrMemory, "wmkcSSL_new",
            "Failed to allocate memory for (*obj).");
    }

    (*obj)->ctx = wmkcNull;
    (*obj)->ssl = wmkcNull;
    (*obj)->net = wmkcNull;

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcSSL_new", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_free WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcSSL_free", "obj is NULL.");
    }

    if((*obj)->ctx) {
        SSL_CTX_free((*obj)->ctx);
    }
    if((*obj)->ssl) {
        SSL_free((*obj)->ssl);
    }
    if((*obj)->net) {
        if((error = wmkcNet_free(&(*obj)->net)).code) {
            return error;
        }
    }
    wmkcMem_free((*obj));

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcSSL_free", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_Context WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj, const SSL_METHOD *_method))
{
    wmkcErr_obj error;
    if(!obj || !_method) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcSSL_Context",
            "obj or _method is NULL.");
    }

    wmkcSize options = SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3;
#   if WMKC_SSL_STRICT_MODE
    options |= (SSL_OP_NO_TLSv1                 | SSL_OP_NO_TLSv1_1);
    options |= (SSL_OP_CIPHER_SERVER_PREFERENCE | SSL_OP_NO_RENEGOTIATION);
#   endif
#ifdef SSL_OP_NO_COMPRESSION
    options |= SSL_OP_NO_COMPRESSION;
#endif
#ifdef SSL_OP_CIPHER_SERVER_PREFERENCE
    options |= SSL_OP_CIPHER_SERVER_PREFERENCE;
#endif
#ifdef SSL_OP_SINGLE_DH_USE
    options |= SSL_OP_SINGLE_DH_USE;
#endif
#ifdef SSL_OP_SINGLE_ECDH_USE
    options |= SSL_OP_SINGLE_ECDH_USE;
#endif
    obj->ctx = SSL_CTX_new(_method);
    if(!SSL_CTX_set_options(obj->ctx, options)) {
        wmkcErr_func_return(error, wmkcErr_Err32, "wmkcSSL_Context",
            "SSL_CTX_set_options error.");
    }
    SSL_CTX_set_cipher_list(obj->ctx, "HIGH:!aNULL:!eNULL");
    SSL_CTX_set_min_proto_version(obj->ctx, TLS1_2_VERSION);
    SSL_CTX_set_mode(obj->ctx, SSL_MODE_RELEASE_BUFFERS);

#define SID_CTX "wmkcSSL"
    SSL_CTX_set_session_id_context(obj->ctx, (const wmkcByte *)SID_CTX, sizeof(SID_CTX));
#undef SID_CTX

    obj->ssl = SSL_new(obj->ctx);

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcSSL_Context", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_wrap_socket WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj, wmkcNet_obj *socket_fd, wmkcCSTR server_hostname))
{
    wmkcErr_obj error;
    if(!obj || !socket_fd) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcSSL_wrap_socket",
            "obj or socket_fd is NULL.");
    }

    obj->net = socket_fd;
    SSL_set_fd(obj->ssl, obj->net->sockfd);
    SSL_set_tlsext_host_name(obj->ssl, server_hostname);

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcSSL_wrap_socket", "OK.");
}



