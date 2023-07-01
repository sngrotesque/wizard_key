#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_HTTP
#define WMKC_HTTP
#include <network/wmkc_net.h>

typedef struct {
    wmkcBool https; // 协议，为True时代表使用SSL/TLS协议，否则HTTP协议
    wmkcChar *host; // 目标主机的域名或IP地址
    wmkc_u16 port; // 目标主机的端口（可选），未使用时请初始化为0
    wmkcChar *path; // URL的路径，默认应为"/"
    wmkcChar *query; // URL查询语句（可选）
} wmkcHttp_obj;


#endif /* WMKC_HTTP */
#endif /* WMKC_SUPPORT */
