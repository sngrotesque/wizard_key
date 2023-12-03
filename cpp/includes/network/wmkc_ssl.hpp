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
            wmkcSSL_Socket()
            {

            }
    };

    class wmkcSSL_Context {
        public:
            wmkcSSL_Context()
            {
                
            }
    };
}

#endif /* WMKC_CPP_SSL */
#endif /* WMKC_SUPPORT */