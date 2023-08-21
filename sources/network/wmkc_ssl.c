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

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_context WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj, const SSL_METHOD *_method))
{
    wmkcErr_obj error;
    if(!obj || !_method) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcSSL_context",
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
    if(!(obj->ctx = SSL_CTX_new(_method))) {
        wmkcErr_func_return(error, wmkcErr_Err32, "wmkcSSL_context",
            "Failed to establish SSL context.");
    }

    SSL_CTX_set_options(obj->ctx, options);
    SSL_CTX_set_cipher_list(obj->ctx, "HIGH:!aNULL:!eNULL");
    SSL_CTX_set_min_proto_version(obj->ctx, TLS1_2_VERSION);
    SSL_CTX_set_mode(obj->ctx, SSL_MODE_RELEASE_BUFFERS);
#define SID_CTX "wmkcSSL"
    SSL_CTX_set_session_id_context(obj->ctx, (const wmkcByte *)SID_CTX, sizeof(SID_CTX));
#undef SID_CTX

    if(!(obj->ssl = SSL_new(obj->ctx))) {
        wmkcErr_func_return(error, wmkcErr_Err32, "wmkcSSL_context",
            "Failed to create a new SSL object.");
    }

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcSSL_context", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_wrap_socket WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj, wmkcNet_obj *socket_fd, wmkcCSTR server_hostname))
{
    wmkcErr_obj error;
    if(!obj || !socket_fd || !server_hostname) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcSSL_wrap_socket",
            "obj or socket_fd or server_hostname is NULL.");
    }

    obj->net = socket_fd;
    SSL_set_fd(obj->ssl, obj->net->sockfd);
    SSL_set_tlsext_host_name(obj->ssl, server_hostname);

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcSSL_wrap_socket", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_settimeout WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj, double _val))
{
    wmkcErr_obj error;

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcSSL_settimeout", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_connect WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj, wmkcCSTR addr, wmkc_u16 port))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcSSL_connect", "obj is NULL.");
    }

    if((error = wmkcNet_connect(obj->net, addr, port)).code) {
        return error;
    }
    SSL_connect(obj->ssl);

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcSSL_connect", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_send WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj, wmkcNetBufT *content, socklen_t size))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcSSL_send", "obj is NULL.");
    }

    if((obj->net->tSize = SSL_write(obj->ssl, content, size)) < 1) {
        wmkcErr_func_return(error, wmkcErr_Err32, "wmkcSSL_send",
            "The socket failed to send data.");
    }

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcSSL_send", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_sendall WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj, wmkcNetBufT *content, wmkcSize size))
{
    wmkcErr_obj error;
    if(!obj || !content || !size) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcSSL_sendall",
            "dst or content or size is NULL.");
    }
    wmkc_u32 retry_count; // 重试次数

    while(size) {
        /* 重传功能
        * 默认情况下，重传次数为5。
        * 但由于TCP协议自带重传功能，并且在不同操作系统中不一样，在Linux中
        * 为15次，在Windows中，基本上为3次或更多。
        * 那么实际的重传次数，可能是75 (15 x 5)或15 (3 x 5)次。
        * 如果5次都失败，那么会直接返回错误信息。
        * 
        * 成功传输的话，会根据每次传输的大小来分割数据包，直到传输完毕。
        */
        retry_count = 5;
        while(retry_count) {
            if((error = wmkcSSL_send(obj, content, size)).code) {
                retry_count--;
            } else {
                break;
            }
        }
        if(!retry_count) {
            return error;
        }

        size -= obj->net->tSize;
        content += obj->net->tSize;
    }

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcSSL_sendall", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_recv WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj, wmkcNetBufT *content, socklen_t size))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcSSL_recv", "obj is NULL.");
    }

    if((obj->net->tSize = SSL_read(obj->ssl, content, size)) < 1) {
        wmkcErr_func_return(error, wmkcErr_Err32, "wmkcSSL_recv",
            "The socket receiving data failed.");
    }

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcSSL_recv", "OK.");
}
