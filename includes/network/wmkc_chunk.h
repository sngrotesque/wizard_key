#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_CHUNK
#define WMKC_CHUNK
#include <wmkc_error.h>
#include <wmkc_memory.h>

#include <network/wmkc_net.h>
#include <crypto/snc.h>
#include <wmkc_struct.h>
#include <wmkc_file.h>

#include <zlib/zlib.h>

WMKC_PUBLIC(wmkcVoid) wmkcChunk_recv WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBuf **buf, socklen_t *size));

WMKC_PUBLIC(wmkcVoid) wmkcChunk_send WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBuf *buf, socklen_t size));

#endif /* WMKC_CHUNK */
#endif /* WMKC_SUPPORT */
