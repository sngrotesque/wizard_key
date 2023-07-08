#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_TP
#define WMKC_TP
#include <wmkc_error.h>
#include <wmkc_basic.h>
#include <wmkc_memory.h>
#include <wmkc_file.h>
#include <wmkc_hash.h>
#include <network/wmkc_net.h>
#include <crypto/snc.h>

#define WMKC_TP_DEFAULT_SDRV_TO 300 // 发送/接受的超时时间
#define WMKC_TP_DEFAULT_BIND_TO 1   // 用于设定SO_REUSEADDR的超时时间
#define WMKC_TP_DEFAULT_MAX_RETRY 5 // 默认最大重试次数为5
#define WMKC_TP_DEFAULT_PORT 49281  // 默认端口

// [chunk:[size][data][md]]
typedef struct {
    wmkcNetSize size;    // 数据长度
    wmkcNetSize chunk_n; // 整个块长度
    wmkcNetBuf *data;    // 数据内容
    wmkcNetBuf *chunk;   // 整个块内容
    wmkcNetBuf md[32];   // SHA-256
} wmkcTp_chunk_obj;

// wmkcTp对象
typedef struct {
    wmkcCSTR addr; // 目标（本地）地址
    wmkc_u16 port; // 目标（本地）端口
    wmkc_u32 maxRetry; // 失败重试次数
    wmkcSNC_obj *snc;  // SNC加密算法对象
    wmkcNet_obj *net;  // wmkcNet对象
    wmkcTp_chunk_obj *chunk; // 内容块
} wmkcTp_obj;

/**
 * @brief 为wmkcTp对象申请内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param obj  指针，指向wmkcTp对象指针的地址
 * @param addr 网络地址，IP或域名
 * @param port 网络端口
 * @param maxRetry 连接失败后的重试次数
 * @param family 网络族[AF_INET，AF_INET6]
 * @param mode   SNC加密算法的模式[SNC_256, SNC_512, SNC_768]
 * @param key    SNC加密算法的密钥
 * @param iv     SNC加密算法的初始向量
 * @return wmkcErr对象code为非0代表出错，需检查。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcTp_new WMKC_OPEN_API
WMKC_OF((wmkcTp_obj **obj, wmkcCSTR addr, wmkc_u16 port, wmkc_u32 maxRetry,
    wmkcNetType family, SNC_mode mode, wmkcByte *key, wmkcByte *iv));

/**
 * @brief 为wmkcTp对象释放内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param obj  指针，指向wmkcTp对象指针的地址
 * @return wmkcErr对象code为非0代表出错，需检查。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcTp_free WMKC_OPEN_API
WMKC_OF((wmkcTp_obj **obj));

#endif
#endif
