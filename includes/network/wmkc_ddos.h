#include <wmkc_conf.h>

#if WMKC_SUPPORT && defined(WMKC_PLATFORM_LINUX)
#ifndef WMKC_DDOS
#define WMKC_DDOS
#include <network/wmkc_net.h>
#include <wmkc_random.h>

#define WMKC_DDOS_IP_ADDR_LEN 16
typedef struct iphdr  IPHDR;
typedef struct tcphdr TCPHDR;

/**
 * @brief DDos函数
 * @authors SN-Grotesque
 * @note 无
 * @param dstIP 目标IP地址
 * @param dstPort 目标端口
 * @param srcIP 源IP地址（为NULL就随机）
 * @param srcPort 源端口（为0就随机）
 * @param count 发送次数
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcDDos_attack WMKC_OPEN_API
WMKC_OF((wmkcCSTR srcIP, wmkc_u16 srcPort, wmkcCSTR dstIP, wmkc_u16 dstPort, wmkcSize count));

#endif /* WMKC_DDOS */
#endif /* WMKC_SUPPORT */
