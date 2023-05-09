/**
 *  后续看看是否要添加重传功能，如果要的话，请把数据加密功能也加上。
 *  同时，请思考到底使用OpenSSL库中的公钥密码算法还是使用自己另外编写的
 *  公钥密码算法来传递对称加密算法的密钥。
 *  
 *  重传次数默认为3次。
 *  如果要加密的话，请将所有内容加密，同时应考虑加密算法的效率问题。
*/

#ifndef __SN_TRANSFER__
#define __SN_TRANSFER__

#include <network/snNet.h>
#include <crypto/snc.h>
#include <snHash.h>

#define SN_FT_SDRV_TO 300 // 发送/接受的超时时间
#define SN_FT_BIND_TO 1   // 用于设定SO_REUSEADDR的超时时间

// 信号：等待
SN_PRIVATE_CONST(snError) SN_FT_SIGNAL_WAIT = 0x74696177U;
// 信号：完成
SN_PRIVATE_CONST(snError) SN_FT_SIGNAL_DONE = 0x656e6f64U;

#define SN_FT_DEFAULT_MAX_RETRY 5 // 默认最大重试次数为5
#define SN_FT_DEFAULT_PORT 49281  // 默认端口

typedef struct {
    const snChar *addr;     // 网络地址
    sn_u16  port;     // 网络端口
    sn_u32  maxRetry; // 失败重试次数
    SNC_ctx *snc;     // SNC加密算法对象
} snTransfer_ctx;

// 为snTransfer对象申请内存空间
SN_PUBLIC(snError) snTransfer_new SN_OPEN_API
SN_FUNC_OF((snTransfer_ctx **ctx, const snChar *addr, sn_u16 port, sn_u32 maxRetry));

// 释放snTransfer对象
SN_PUBLIC(snError) snTransfer_release SN_OPEN_API
SN_FUNC_OF((snTransfer_ctx **ctx));

// 接收端
SN_PUBLIC(snError) snTransfer_Listen SN_OPEN_API
SN_FUNC_OF((snTransfer_ctx *ctx, const snChar *fn));

// 发送端
SN_PUBLIC(snError) snTransfer_Client SN_OPEN_API
SN_FUNC_OF((snTransfer_ctx *ctx, const snChar *fn));

#endif // #ifndef __SN_TRANSFER__
