#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_SSL
#define WMKC_SSL
#include <wmkc_error.h>
#include <network/wmkc_net.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/sslerr.h>
#include <openssl/crypto.h>

#define WMKC_SSL_STRICT_MODE true

typedef struct {
	SSL *ssl;         // SSL对象
	SSL_CTX *ctx;     // SSL上下文对象
	wmkcNet_obj *net; // wmkcNet对象
} wmkcSSL_obj;

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_new WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj **obj));

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_free WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj **obj));

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_context WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj, const SSL_METHOD *_method));

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_wrap_socket WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj, wmkcNet_obj *socket_fd, wmkcCSTR server_hostname));

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_settimeout WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj, double _val));

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_connect WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj, wmkcCSTR addr, wmkc_u16 port));

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_send WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj, wmkcNetBufT *content, socklen_t size));

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_sendall WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj, wmkcNetBufT *content, wmkcSize size));

WMKC_PUBLIC(wmkcErr_obj) wmkcSSL_recv WMKC_OPEN_API
WMKC_OF((wmkcSSL_obj *obj, wmkcNetBufT *content, socklen_t size));


#endif /* WMKC_SSL */
#endif /* WMKC_SUPPORT */
