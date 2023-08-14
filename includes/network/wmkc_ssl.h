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

typedef struct {
	SSL *ssl;         // SSL对象
	SSL_CTX *ctx;     // SSL上下文对象
	wmkcNet_obj *net; // wmkcNet对象
} wmkcSSL_obj;

#endif /* WMKC_SSL */
#endif /* WMKC_SUPPORT */
