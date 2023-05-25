#include <network/wmkc_net.h>

// 申请wmkcNet对象的内存空间
WMKC_PUBLIC(wmkcErr_ctx) wmkcNet_new WMKC_OPEN_API
WMKC_OF((wmkcNet_ctx **net, wmkc_u32 family))
{
    wmkcErr_ctx error;

    if(!net) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_new: net is NULL.");
    }

    if(!wmkcMemoryNew(wmkcNet_ctx *, (*net), sizeof(wmkcNet_ctx))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcNet_new: (*net) Failed to apply for memory.");
    }

    // 为结构体socket成员设定类型并将网络结构大小赋值
    // 如果即不是IPv4也不是IPv6就返回错误代码
    if(family == AF_INET) {
        (*net)->sockfdFamily = AF_INET;
        (*net)->addr_info_size = WMKC_NET_IPV4_ADDR_SIZE;
    } else if(family == AF_INET6) {
        (*net)->sockfdFamily = AF_INET6;
        (*net)->addr_info_size = WMKC_NET_IPV6_ADDR_SIZE;
    } else {
        wmkcErr_return(error, wmkcErr_NetFamily,
            "wmkcNet_new: The type of socket must be AF_INET or AF_INET6.");
    }
    if(!wmkcMemoryNew(SOCKADDR *, (*net)->addr_info, (*net)->addr_info_size)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcNet_new: (*net)->addr_info Failed to apply for memory.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_ctx) wmkcNet_free WMKC_OPEN_API
WMKC_OF((wmkcNet_ctx **net))
{
    wmkcErr_ctx error;
    if(!net) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_free: net is NULL.");
    }

    if((*net)->addr_info) {
        wmkcMemoryFree((*net)->addr_info);
    }
    if((*net)) {
        wmkcMemoryFree((*net));
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

// 初始化wmkcNet对象（需提前申请内存空间）
WMKC_PUBLIC(wmkcErr_ctx) wmkcNet_init WMKC_OPEN_API
WMKC_OF((wmkcNet_ctx *net, wmkcString hostname, wmkc_u16 port, wmkcBool UDP))
{
    wmkcErr_ctx error;

    if(!net) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_init: net is NULL.");
    }
    if((net->sockfdFamily != AF_INET) && (net->sockfdFamily != AF_INET6)) {
        wmkcErr_return(error, wmkcErr_NetFamily,
            "wmkcNet_init: The type of socket must be AF_INET or AF_INET6.");
    }

#   if defined(WMKC_WINDOWS_SUPPORT)
    // 如果是Windows系统就初始化并启动WSADATA对象。
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws)) {
        wmkcErr_return(error, wmkcErr_NetWsaData, "wmkcNet_init: WSADATA startup failed.");
    }
#   endif

    if(hostname) {
        SOCKADDR_IN *ipv4 = wmkcNull; // 一个IPv4网络结构的指针
        SOCKADDR_IN6 *ipv6 = wmkcNull; // 一个IPv6网络结构的指针

        if(UDP) {
            // 如果Socket是UDP类型
            net->sockfd = wmkcNet_UDP_Socket(net->sockfdFamily, IPPROTO_IP);
            net->sockfdType = WMKC_NET_UDP_TYPE;
        } else {
            // 否则Socket是TCP类型
            net->sockfd = wmkcNet_TCP_Socket(net->sockfdFamily, IPPROTO_IP);
            net->sockfdType = WMKC_NET_TCP_TYPE;
        }
        // 如果Socket申请失败就返回错误代码
        if(net->sockfd == wmkcErr_Err32) {
            wmkcErr_return(error, wmkcErr_NetSocket,
                "wmkcNet_init: Unable to create socket.");
        }

        // 解析域名，如果失败就返回错误代码
        error = wmkcNet_resolveAddress(net, hostname);
        if(error.code) {
            return error;
        }

        if(net->sockfdFamily == AF_INET) {
            // 如果是IPv4网络结构，那么使用IPv4网络结构的指针来设定端口号
            ipv4 = (SOCKADDR_IN *)net->addr_info;
            ipv4->sin_port = htons(port);
        } else if(net->sockfdFamily == AF_INET6) {
            // 如果是IPv6网络结构，那么使用IPv6网络结构的指针来设定端口号
            ipv6 = (SOCKADDR_IN6 *)net->addr_info;
            ipv6->sin6_port = htons(port);
        }
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

// 域名解析函数
WMKC_PUBLIC(wmkcErr_ctx) wmkcNet_resolveAddress
WMKC_OF((wmkcNet_ctx *net, wmkcString hostname))
{
    wmkcErr_ctx error;

    if(!net || !hostname || !net->addr_info) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcNet_resolveAddress: net or hostname or net->addr_info is NULL.");
    }

    // 判断用户输入的网络家族是IPv4还是IPv6，如果都不是就返回错误代码
    if((net->sockfdFamily != AF_INET) && (net->sockfdFamily != AF_INET6)) {
        wmkcErr_return(error, wmkcErr_NetFamily,
            "wmkcNet_resolveAddress: The type of socket must be AF_INET or AF_INET6.");
    }

    ADDRINFO *result = wmkcNull;
    ADDRINFO hints = {.ai_family = net->sockfdFamily};

    // 进行域名解析，解析失败就返回错误代码
    if(getaddrinfo(hostname, "echo", &hints, &result)) {
        wmkcErr_return(error, wmkcErr_NetDomainResolv,
            "wmkcNet_resolveAddress: Domain Name System failed.");
    }

    // 将解析结果保存至结构体成员中
    memcpy(net->addr_info, result->ai_addr, net->addr_info_size);

    freeaddrinfo(result); // 释放掉已结束使用的解析数据
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

// 设置发送与接收超时时间
WMKC_PUBLIC(wmkcErr_ctx) wmkcNet_timeout WMKC_OPEN_API
WMKC_OF((wmkcNet_ctx *net, wmkcNetTime _user_TimeOut))
{
    wmkcErr_ctx error;
    if(!net) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_timeout: net is NULL.");
    }

    wmkcNetSize optlen = sizeof(struct timeval);
    wmkcNetTimer *optval = wmkcNull;
    struct timeval _timeout;

    _timeout.tv_sec = floor(_user_TimeOut);
    _timeout.tv_usec = (long)(modf(_user_TimeOut, &_user_TimeOut) * 1000);

    if(_user_TimeOut) {
        optval = (wmkcNetTimer *)&_timeout;
        if(setsockopt(net->sockfd, SOL_SOCKET, SO_SNDTIMEO, optval, optlen) ||
            setsockopt(net->sockfd, SOL_SOCKET, SO_RCVTIMEO, optval, optlen)) {
            wmkcErr_return(error, wmkcErr_NetSetSockOpt,
                "wmkcNet_timeout: Error in setsockopt function.");
        }
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

// 连接函数
WMKC_PUBLIC(wmkcErr_ctx) wmkcNet_connect WMKC_OPEN_API
WMKC_OF((wmkcNet_ctx *net))
{
    wmkcErr_ctx error;
    if(!net) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_connect: net is NULL.");
    }
    if(net->sockfdType == WMKC_NET_UDP_TYPE) {
        wmkcErr_return(error, wmkcErr_NetSockfdType,
            "wmkcNet_connect: Wrong socket type, currently not expected to be UDP.");
    }

    // 连接至接收端，失败就返回错误代码
    if(connect(net->sockfd, net->addr_info, net->addr_info_size) == wmkcErr_Err32) {
        wmkcErr_return(error, wmkcErr_NetConnect,
            "wmkcNet_connect: Socket connection failed.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

// 绑定函数
WMKC_PUBLIC(wmkcErr_ctx) wmkcNet_bind WMKC_OPEN_API
WMKC_OF((wmkcNet_ctx *net, wmkcNetTime _user_TimeOut))
{
    wmkcErr_ctx error;
    if(!net) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_bind: net is NULL.");
    }

    wmkcNetTimer *optval = wmkcNull;
    struct timeval _timeout;

    if(_user_TimeOut) {
        _timeout.tv_sec = floor(_user_TimeOut);
        _timeout.tv_usec = (long)(modf(_user_TimeOut, &_user_TimeOut) * 1000);
        optval = (wmkcNetTimer *)&_timeout;

        if(setsockopt(net->sockfd, SOL_SOCKET, SO_REUSEADDR, optval, sizeof(_timeout))) {
            wmkcErr_return(error, wmkcErr_NetSetSockOpt,
                "wmkcNet_bind: Error in setsockopt function in wmkcNet_timeout function.");
        }
    }

    // 如果绑定失败就返回错误代码
    if(bind(net->sockfd, net->addr_info, net->addr_info_size) == wmkcErr_Err32) {
        wmkcErr_return(error, wmkcErr_NetBind, "wmkcNet_bind: Socket binding failed.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

// 监听函数
WMKC_PUBLIC(wmkcErr_ctx) wmkcNet_listen WMKC_OPEN_API
WMKC_OF((wmkcNet_ctx *net, wmkc_u32 _Listen))
{
    wmkcErr_ctx error;
    if(!net) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_listen: net is NULL.");
    }

    // 监听连接，失败或者Socket类型不正确就返回错误代码
    if(net->sockfdType == WMKC_NET_TCP_TYPE) {
        if(!_Listen) {
            _Listen = WMKC_NET_DEFAULT_LISTEN;
        }
        if(listen(net->sockfd, _Listen) == wmkcErr_Err32) {
            wmkcErr_return(error, wmkcErr_NetListen,
                "wmkcNet_listen: Socket listening failed.");
        }
    } else {
        wmkcErr_return(error, wmkcErr_NetSockfdType,
            "wmkcNet_listen: Wrong socket type, currently not expected to be UDP.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

// 等待连接函数
WMKC_PUBLIC(wmkcErr_ctx) wmkcNet_accept WMKC_OPEN_API
WMKC_OF((wmkcNet_ctx *dst, wmkcNet_ctx *src))
{
    wmkcErr_ctx error;
    if(!dst || !src) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_accept: dst or src is NULL.");
    }
    if(!dst->addr_info || !src->addr_info) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcNet_accept: dst->addr_info or src->addr_info is NULL.");
    }

    // 接收连接，失败就返回错误代码
    if(src->sockfdType == WMKC_NET_TCP_TYPE) {
        dst->sockfd = accept(src->sockfd, dst->addr_info, &dst->addr_info_size);
        if(dst->sockfd == wmkcErr_Err32) {
            wmkcErr_return(error, wmkcErr_NetAccept,
                "wmkcNet_accept: The socket failed to accept the connection.");
        }
    } else {
        wmkcErr_return(error, wmkcErr_NetSockfdType,
            "wmkcNet_accept: Wrong socket type, currently not expected to be UDP.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

// 发送函数
WMKC_PUBLIC(wmkcErr_ctx) wmkcNet_send WMKC_OPEN_API
WMKC_OF((wmkcNet_ctx *net, wmkcNetSize *_tSize, wmkcNetBuf *buf, wmkcNetSize size))
{
    wmkcErr_ctx error;
    if(!net || !buf || !size) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcNet_send: net or buf or size is NULL.");
    }
    if(!_tSize) {
        wmkcNetSize size;
        _tSize = &size;
    }

    // 判断Socket类型，TCP或UDP。发送数据。如果返回错误代码就返回对应的错误代码。
    switch(net->sockfdType) {
        case WMKC_NET_TCP_TYPE: // TCP
            *_tSize = send(net->sockfd, buf, size, 0);
            if(*_tSize == wmkcErr_Err32) {
                wmkcErr_return(error, wmkcErr_NetSend,
                    "wmkcNet_send: The socket failed to send data.");
            }
            break;
        case WMKC_NET_UDP_TYPE: // UDP
            *_tSize = sendto(net->sockfd, buf, size, 0,
                net->addr_info, net->addr_info_size);
            if(*_tSize == wmkcErr_Err32) {
                wmkcErr_return(error, wmkcErr_NetSend,
                    "wmkcNet_send: The socket failed to send data.");
            }
            break;
        default:
            wmkcErr_return(error, wmkcErr_NetSockfdType,
                "wmkcNet_send: Wrong socket type, currently not expected to be UDP.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

// 全部发送函数
WMKC_PUBLIC(wmkcErr_ctx) wmkcNet_sendall WMKC_OPEN_API
WMKC_OF((wmkcNet_ctx *net, wmkcNetBuf *buf, wmkcNetSize size))
{
    wmkcErr_ctx error;
    if(!net || !buf || !size) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcNet_sendall: net or buf or size is NULL.");
    }
    wmkcNetSize index, quotient, leftover, _tSize;

    // 计算需要发送WMKC_NET_BLOCKLEN大小的数据包的次数
    quotient = size / WMKC_NET_BLOCKLEN;
    // 计算是否还有剩下的数据包
    leftover = size % WMKC_NET_BLOCKLEN;

    for(index = 0; index < quotient; ++index) {
        // 如果发送错误就返回错误代码
        error = wmkcNet_send(net, &_tSize, buf, WMKC_NET_BLOCKLEN);
        if(error.code) {
            return error;
        }
        buf += WMKC_NET_BLOCKLEN;
    }

    // 如果有剩下的数据包，那么就发送。如果发送错误就返回错误代码
    if(leftover) {
        error = wmkcNet_send(net, &_tSize, buf, leftover);
        if(error.code) {
            return error;
        }
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

// 接收函数
WMKC_PUBLIC(wmkcErr_ctx) wmkcNet_recv WMKC_OPEN_API
WMKC_OF((wmkcNet_ctx* net, wmkcNetSize *_tSize, wmkcNetBuf *buf, wmkcNetSize size))
{
    wmkcErr_ctx error;
    if(!net || !buf || !size) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcNet_recv: net or buf or size is NULL.");
    }
    if(!_tSize) {
        wmkcNetSize size;
        _tSize = &size;
    }

    switch(net->sockfdType) {
        case WMKC_NET_TCP_TYPE:
            *_tSize = recv(net->sockfd, buf, size, 0);
            if(*_tSize == wmkcErr_Err32) {
                wmkcErr_return(error, wmkcErr_NetRecv,
                    "wmkcNet_recv: The socket failed to receive data.");
            }
            break;
        case WMKC_NET_UDP_TYPE:
            *_tSize = recvfrom(net->sockfd, buf, size, 0,
                net->addr_info, &net->addr_info_size);
            if(*_tSize == wmkcErr_Err32) {
                wmkcErr_return(error, wmkcErr_NetRecv,
                    "wmkcNet_recv: The socket failed to receive data.");
            }
            break;
        default:
            wmkcErr_return(error, wmkcErr_NetSockfdType,
                "wmkcNet_recv: Wrong socket type, currently not expected to be UDP.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

// 关闭套接字函数
WMKC_PUBLIC(wmkcErr_ctx) wmkcNet_close WMKC_OPEN_API
WMKC_OF((wmkcNet_ctx *net))
{
    wmkcErr_ctx error;
    if(!net) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_close: net is NULL.");
    }

    // 判断系统类型，根据对应系统使用不同的关闭函数将Socket与WSADATA进行关闭
    // 如果错误，就返回错误代码
#if defined(WMKC_LINUX_SUPPORT)
    if((shutdown(net->sockfd, 2) | close(net->sockfd)) == wmkcErr_Err32) {
        wmkcErr_return(error, wmkcErr_NetClose, "wmkcNet_close: Socket close failed.");
    }
#elif defined(WMKC_WINDOWS_SUPPORT)
    if((closesocket(net->sockfd) | WSACleanup()) == wmkcErr_Err32) {
        wmkcErr_return(error, wmkcErr_NetClose, "wmkcNet_close: Socket close failed.");
    }
#endif

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
