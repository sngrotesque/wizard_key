/**
 * 首先是热烈祝贺自己终于把这个网络库给写完了
 * 
 * 其次是，明天醒来的时候，再次检查一下有没有哪里需要修改的。
 * 如果能改，那尽量给它改了。
 * 
 * 还有一些细节方面的问题，请一定要切换系统来编写这个针对性的跨平台代码。
 * 
 * 最后呢，查找一下有没有哪里可能存在漏洞的地方，如果有，把它修复掉。
 * 
 * 晚安，自己。
*/
#include <network/snNet.h>

// 域名解析函数
SN_PRIVATE(snError) snNet_HostnameToIP
SN_FUNC_OF((snNet_ctx *ctx, snString hostname))
{
    // 判断用户输入的网络家族是IPv4还是IPv6，如果都不是就返回错误代码
    if(ctx->sockfdFamily != AF_INET && ctx->sockfdFamily != AF_INET6) {
        return snErr_NetFamily;
    }
    // 如果用于储存数据的类型为空就返回错误代码
    if(!ctx->info->receiver ||
        (ctx->info->size < SN_NET_IPV4_ADDR_SIZE &&
        ctx->info->size < SN_NET_IPV6_ADDR_SIZE)) {
        return snErr_Memory;
    }
    // 用来储存解析结果
    ADDRINFO *result = snNull;
    // 使用用户指定的网络家族（IPv4 or IPv6）
    ADDRINFO hints = {.ai_family = ctx->sockfdFamily};

    // 进行域名解析，解析失败就返回错误代码
    if(getaddrinfo(hostname, "0", &hints, &result)) {
        return snErr_NetDomainResolv;
    }

    // 将解析结果保存至结构体成员中
    memcpy(ctx->info->receiver, result->ai_addr, ctx->info->size);
    // 释放掉已结束使用的解析数据
    freeaddrinfo(result);

    return snErr_OK;
}

SN_PUBLIC(snError) snNet_new SN_OPEN_API
SN_FUNC_OF((snNet_ctx **ctx, sn_u32 family))
{
    // 如果申请snNet对象的内存失败
    if(!((*ctx) = (snNet_ctx *)malloc(sizeof(snNet_ctx)))) {
        return snErr_Memory;
    }
    // 如果为snNet对象的info成员申请内存失败
    if(!((*ctx)->info  = (snNetInfo *)malloc(sizeof(snNetInfo)))) {
        return snErr_Memory;
    }

    // 为结构体socket成员设定类型并将网络结构大小赋值
    // 如果即不是IPv4也不是IPv6就返回错误代码
    if(family == AF_INET) {
        (*ctx)->sockfdFamily = AF_INET;
        (*ctx)->info->size = SN_NET_IPV4_ADDR_SIZE;
    } else if(family == AF_INET6) {
        (*ctx)->sockfdFamily = AF_INET6;
        (*ctx)->info->size = SN_NET_IPV6_ADDR_SIZE;
    } else {
        return snErr_NetFamily;
    }

    // 为网络结构成员申请内存空间
    (*ctx)->info->receiver = (SOCKADDR *)malloc((*ctx)->info->size);
    (*ctx)->info->client = (SOCKADDR *)malloc((*ctx)->info->size);
    // 如果为网络结构成员申请内存失败
    if(!(*ctx)->info->receiver || !(*ctx)->info->client) {
        return snErr_Memory;
    }

    return snErr_OK;
}

SN_PUBLIC(snError) snNet_release SN_OPEN_API
SN_FUNC_OF((snNet_ctx **ctx))
{
    // 如果指针指向非空地址，那么将其释放掉
    // 请确保你的指针指向正确的地址。
    if((*ctx)) {
        if((*ctx)->info) {
            if((*ctx)->info->receiver)
                free((*ctx)->info->receiver);
            if((*ctx)->info->client)
                free((*ctx)->info->client);
            free((*ctx)->info);
        }
        free((*ctx));
        (*ctx) = snNull;
    }
    return snErr_OK;
}

// 初始化snNet对象（需提前申请内存空间）
SN_PUBLIC(snError) snNet_init SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, snString hostname, sn_u16 port, snBool UDP))
{
#   if defined(_WIN32) // 如果是Windows系统就初始化并启动WSADATA对象。
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws)) {
        return snErr_NetWsaData;
    }
#   endif

    // 一个IPv4网络结构的指针
    static SOCKADDR_IN *ipv4 = snNull;
    // 一个IPv6网络结构的指针
    static SOCKADDR_IN6 *ipv6 = snNull;
    // 错误代码
    static snError code;

    if(UDP) {
        // 如果Socket是UDP类型
        ctx->sockfd = snNet_UDP_Socket(ctx->sockfdFamily, IPPROTO_IP);
        ctx->sockfdType = SN_NET_UDP_TYPE;
    } else {
        // 否则Socket是TCP类型
        ctx->sockfd = snNet_TCP_Socket(ctx->sockfdFamily, IPPROTO_IP);
        ctx->sockfdType = SN_NET_TCP_TYPE;
    }
    // 如果Socket申请失败就返回错误代码
    if(ctx->sockfd == snErr_Err32) {
        return snErr_NetSocket;
    }

    // 解析域名，如果失败就返回错误代码
    code = snNet_HostnameToIP(ctx, hostname);
    if(code) {
        return code;
    }

    if(ctx->sockfdFamily == AF_INET) {
        // 如果是IPv4网络结构，那么使用IPv4网络结构的指针来设定端口号
        ipv4 = (SOCKADDR_IN *)ctx->info->receiver;
        ipv4->sin_port = htons(port);
    } else if(ctx->sockfdFamily == AF_INET6) {
        // 如果是IPv6网络结构，那么使用IPv6网络结构的指针来设定端口号
        ipv6 = (SOCKADDR_IN6 *)ctx->info->receiver;
        ipv6->sin6_port = htons(port);
    }

    return snErr_OK;
}

// 设置发送与接收超时时间
SN_PUBLIC(snError) snNet_timeout SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, snNetTime _user_TimeOut))
{
    struct timeval _time_out = {
        // 取出整数部分作为tv_sec成员
        .tv_sec = floor(_user_TimeOut),
        // 取出小数部分作为tv_usec成员
        .tv_usec = (snSize)(modf(_user_TimeOut, &_user_TimeOut) * 1000)
    };
    // 将时间结构作为指针传递到setsockopt函数内
    snNetTimer *_timer_ptr = (snNetTimer *)&_time_out;
    if(setsockopt(ctx->sockfd, SOL_SOCKET, SO_SNDTIMEO, _timer_ptr, sizeof(_time_out)) ||
        setsockopt(ctx->sockfd, SOL_SOCKET, SO_RCVTIMEO, _timer_ptr, sizeof(_time_out))) {
        return snErr_NetSetSockOpt;
    }
    return snErr_OK;
}

// 连接函数
SN_PUBLIC(snError) snNet_connect SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx))
{
    // 连接至接收端，失败就返回错误代码
    if(connect(ctx->sockfd, ctx->info->receiver, ctx->info->size) == snErr_Err32) {
        return snErr_NetConnect;
    }
    return snErr_OK;
}

// 绑定函数
SN_PUBLIC(snError) snNet_bind SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, snNetTime _user_TimeOut))
{
    // 将时间结构作为指针传递到setsockopt函数内
    snNetTimer *_timer_ptr = snNull;
    // 用作设置超时时间的结构体
    struct timeval _time_out;

    if(_user_TimeOut) {
        // 取出整数部分作为tv_sec成员
        _time_out.tv_sec = floor(_user_TimeOut);
        // 取出小数部分作为tv_usec成员
        _time_out.tv_usec = (snSize)(modf(_user_TimeOut, &_user_TimeOut) * 1000);
        _timer_ptr = (snNetTimer *)&_time_out;
        // 用于设定SO_REUSEADDR
        if(setsockopt(ctx->sockfd, SOL_SOCKET, SO_REUSEADDR, _timer_ptr, sizeof(_time_out))) {
            return snErr_NetSetSockOpt;
        }
    }

    // 如果绑定失败就返回错误代码
    if(bind(ctx->sockfd, ctx->info->receiver, ctx->info->size) == snErr_Err32) {
        return snErr_NetBind;
    }

    return snErr_OK;
}

// 监听函数
SN_PUBLIC(snError) snNet_listen SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, sn_u32 _Listen))
{
    // 监听连接，失败或者Socket类型不正确就返回错误代码
    if(ctx->sockfdType == SN_NET_TCP_TYPE) {
        if(listen(ctx->sockfd, _Listen) == snErr_Err32)
            return snErr_NetListen;
    } else {
        return snErr_NetSockfdType;
    }

    return snErr_OK;
}

// 等待连接函数
SN_PUBLIC(snError) snNet_accept SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx))
{
    // 接收连接，失败就返回错误代码
    snNetSocket _tmp_socket = ctx->sockfd;
    if(ctx->sockfdType == SN_NET_TCP_TYPE) {
        ctx->sockfd = accept(_tmp_socket, ctx->info->client, &ctx->info->size);
        if(ctx->sockfd == snErr_Err32)
            return snErr_NetAccept;
    } else {
        return snErr_NetSockfdType;
    }

#if defined(__linux)
    shutdown(_tmp_socket, 2);
    close(_tmp_socket);
#elif defined(_WIN32)
    closesocket(_tmp_socket);
#endif

    return snErr_OK;
}

// 发送函数
SN_PUBLIC(snError) snNet_send SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, snNetSize *_tSize, snNetBuf *buf, snNetSize size))
{
    // 判断Socket类型，TCP或UDP。发送数据。如果返回错误代码就返回对应的错误代码。
    if(ctx->sockfdType == SN_NET_TCP_TYPE) {
        *_tSize = send(ctx->sockfd, buf, size, 0);
        if(*_tSize == snErr_Err32)
            return snErr_NetSend;
    } else if(ctx->sockfdType == SN_NET_UDP_TYPE) {
        *_tSize = sendto(ctx->sockfd, buf, size, 0,
            ctx->info->receiver, ctx->info->size);
        if(*_tSize == snErr_Err32)
            return snErr_NetSend;
    }
    return snErr_OK;
}

// 全部发送函数
SN_PUBLIC(snError) snNet_sendall SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, snNetBuf *buf, snNetSize size))
{
    static snNetSize index, quotient, leftover, _tSize;

    // 计算需要发送SN_NET_BLOCKLEN大小的数据包的次数
    quotient = size / SN_NET_BLOCKLEN;
    // 计算是否还有剩下的数据包
    leftover = size % SN_NET_BLOCKLEN;

    for(index = 0; index < quotient; ++index) {
        // 如果发送错误就返回错误代码
        if(snNet_send(ctx, &_tSize, buf, SN_NET_BLOCKLEN) == snErr_NetSend)
            return snErr_NetSend;
        buf += SN_NET_BLOCKLEN;
    }

    // 如果有剩下的数据包，那么就发送。如果发送错误就返回错误代码
    if(leftover)
        if(snNet_send(ctx, &_tSize, buf, leftover) == snErr_NetSend)
            return snErr_NetSend;

    return snErr_OK;
}

// 接收函数
SN_PUBLIC(snError) snNet_recv SN_OPEN_API
SN_FUNC_OF((snNet_ctx* ctx, snNetSize *_tSize, snNetBuf *buf, snNetSize size))
{
    // 判断Socket类型，TCP或UDP。接收数据。如果返回错误代码就返回对应的错误代码。
    if(ctx->sockfdType == SN_NET_TCP_TYPE) {
        *_tSize = recv(ctx->sockfd, buf, size, 0);
        if(*_tSize == snErr_Err32)
            return snErr_NetRecv;
    } else if(ctx->sockfdType == SN_NET_UDP_TYPE) {
        *_tSize = recvfrom(ctx->sockfd, buf, size, 0,
            ctx->info->receiver, &ctx->info->size);
        if(*_tSize == snErr_Err32)
            return snErr_NetRecv;
    }
    return snErr_OK;
}

// 关闭套接字函数
SN_PUBLIC(snError) snNet_close SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx))
{
    // 判断系统类型，根据对应系统使用不同的关闭函数将Socket与WSADATA（如果有的话）进行关闭
    // 如果错误，就返回错误代码
#if defined(__linux)
    if((shutdown(ctx->sockfd, 2) | close(ctx->sockfd)) == snErr_Err32) {
        return snErr_NetClose;
    }
#elif defined(_WIN32)
    if(closesocket(ctx->sockfd) || WSACleanup())
        return snErr_NetClose;
#endif
    return snErr_OK;
}
