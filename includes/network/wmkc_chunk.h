#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_CHUNK
#define WMKC_CHUNK
#include <wmkc_error.h>
#include <wmkc_memory.h>

#include <network/wmkc_net.h>
#include <crypto/snc.h>
#include <wmkc_struct.h>
#include <wmkc_stream.h>
#include <wmkc_hash.h>
#include <wmkc_pad.h>

#include <zlib/zlib.h>

typedef struct {
    wmkcByte  packetNumber[4]; // 数据包编号
    wmkcByte  packetSize[4];   // 数据包长度
    wmkcByte *packetContent;   // 数据包内容
    wmkcByte  packetCRC[4];    // 数据包校验
    wmkc_u32  packetSize_n;    // 数据包长度：整数
} wmkcChunk_obj;

#endif /* WMKC_CHUNK */
#endif /* WMKC_SUPPORT */
