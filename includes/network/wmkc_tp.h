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
    wmkcCSTR addr;    // 网络地址
    wmkc_u16  port;     // 网络端口
    wmkc_u32  maxRetry; // 失败重试次数
    wmkcSNC_obj *snc;       // SNC加密算法对象
} wmkcTp_obj;

/**
 * @brief 为wmkcTp对象申请内存空间
 * @authors SN-Grotesque
 * @note 未完成
 * @param obj 这是一个指针，指向wmkcTp对象指针的地址。
 * @param addr 这是一个指针，指向目标主机的域名（IP地址）字符串。
 * @param port 这是一个端口，为目标主机的网络端口。
 * @param maxRetry 这是一个数字，代表最大重试次数，为0时使用
 *                 WMKC_TP_DEFAULT_MAX_RETRY的值。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcTp_new WMKC_OPEN_API
WMKC_OF((wmkcTp_obj **obj, wmkcCSTR addr, wmkc_u16 port, wmkc_u32 maxRetry));

/**
 * @brief 为wmkcTp对象释放内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param obj 这是一个指针，指向wmkcTp对象指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcTp_free WMKC_OPEN_API
WMKC_OF((wmkcTp_obj **obj));

/**
 * @brief 接收数据端函数
 * @authors SN-Grotesque
 * @note 未完成
 * @param obj 这是一个指针，指向wmkcTp对象的地址。
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcTp_Listen WMKC_OPEN_API
WMKC_OF((wmkcTp_obj *obj, wmkcCSTR fn));

/**
 * @brief 发送数据端函数
 * @authors SN-Grotesque
 * @note 未完成
 * @param obj 这是一个指针，指向wmkcTp对象的地址。
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcTp_Client WMKC_OPEN_API
WMKC_OF((wmkcTp_obj *obj, wmkcCSTR fn));

#endif
#endif
