/**
 *  后续看看是否要添加重传功能，如果要的话，请把数据加密功能也加上。
 *  同时，请思考到底使用OpenSSL库中的公钥密码算法还是使用自己另外编写的
 *  公钥密码算法来传递对称加密算法的密钥。
 *  
 *  重传次数默认为3次。
 *  如果要加密的话，请将所有内容加密，同时应考虑加密算法的效率问题。
*/

//* 预计6月恢复此库的编写

#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_TRANSFER
#define WMKC_TRANSFER
#include <wmkc_error.h>
#include <wmkc_memory.h>
#include <wmkc_file.h>
#include <wmkc_hash.h>
#include <network/wmkc_net.h>
#include <crypto/snc.h>

#define WMKC_TP_DEFAULT_SDRV_TO 300 // 发送/接受的超时时间
#define WMKC_TP_DEFAULT_BIND_TO 1   // 用于设定SO_REUSEADDR的超时时间
#define WMKC_TP_DEFAULT_MAX_RETRY 5 // 默认最大重试次数为5
#define WMKC_TP_DEFAULT_PORT 49281  // 默认端口

// 信号：等待
WMKC_PRIVATE_CONST(wmkc_u32) WMKC_TP_SIGNAL_WAIT = 0x74696177U;
// 信号：完成
WMKC_PRIVATE_CONST(wmkc_u32) WMKC_TP_SIGNAL_DONE = 0x656e6f64U;

// wmkcTp对象
typedef struct {
    wmkcCSTR addr; // 目标（本地）地址
    wmkc_u16 port; // 目标（本地）端口
    wmkc_u32 maxRetry; // 失败重试次数
    wmkcSNC_obj *snc;  // SNC加密算法对象
    wmkcNet_obj *net;  // wmkcNet对象
} wmkcTp_obj;

WMKC_PUBLIC(wmkcErr_obj) wmkcTp_init WMKC_OPEN_API
WMKC_OF((wmkcCSTR addr, wmkc_u16 port, wmkc_u32 msxRetry, SNC_mode mode));

#endif
#endif
