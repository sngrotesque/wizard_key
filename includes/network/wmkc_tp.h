/**
 *  后续看看是否要添加重传功能，如果要的话，请把数据加密功能也加上。
 *  同时，请思考到底使用OpenSSL库中的公钥密码算法还是使用自己另外编写的
 *  公钥密码算法来传递对称加密算法的密钥。
 *  
 *  重传次数默认为3次。
 *  如果要加密的话，请将所有内容加密，同时应考虑加密算法的效率问题。
*/

//* 预计6月恢复此库的编写

#ifndef WMKC_TRANSFER
#define WMKC_TRANSFER

#include <wmkc_file.h>
#include <wmkc_hash.h>
#include <network/wmkc_net.h>
#include <crypto/snc.h>

#define WMKC_TP_SDRV_TO 300 // 发送/接受的超时时间
#define WMKC_TP_BIND_TO 1   // 用于设定SO_REUSEADDR的超时时间

// 信号：等待
WMKC_PRIVATE_CONST(wmkc_u32) WMKC_TP_SIGNAL_WAIT = 0x74696177U;
// 信号：完成
WMKC_PRIVATE_CONST(wmkc_u32) WMKC_TP_SIGNAL_DONE = 0x656e6f64U;

#define WMKC_TP_DEFAULT_MAX_RETRY 5 // 默认最大重试次数为5
#define WMKC_TP_DEFAULT_PORT 49281  // 默认端口

typedef struct {
    wmkcString addr;    // 网络地址
    wmkc_u16  port;     // 网络端口
    wmkc_u32  maxRetry; // 失败重试次数
    SNC_ctx *snc;       // SNC加密算法对象
} wmkcTp_obj;

// 为wmkcTp对象申请内存空间
WMKC_PUBLIC(wmkcErr_obj) wmkcTp_new WMKC_OPEN_API
WMKC_OF((wmkcTp_obj **net, wmkcString addr, wmkc_u16 port, wmkc_u32 maxRetry));

// 释放wmkcTp对象
WMKC_PUBLIC(wmkcErr_obj) wmkcTp_free WMKC_OPEN_API
WMKC_OF((wmkcTp_obj **net));

// 接收端
WMKC_PUBLIC(wmkcErr_obj) wmkcTp_Listen WMKC_OPEN_API
WMKC_OF((wmkcTp_obj *net, wmkcFileString fn));

// 发送端
WMKC_PUBLIC(wmkcErr_obj) wmkcTp_Client WMKC_OPEN_API
WMKC_OF((wmkcTp_obj *net, wmkcFileString fn));

#endif
