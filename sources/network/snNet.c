#include <network/snNet.h>

// 申请snNet对象的内存空间
SN_PUBLIC(snErr_ctx) snNet_new SN_OPEN_API
SN_FUNC_OF((snNet_ctx **net, sn_u32 family))
{
    snErr_ctx error;

    if(!net) {
        snErr_return(error, snErr_ErrNULL, "snNet_new: net is NULL.");
    }

    if(!snMemoryNew(snNet_ctx *, (*net), sizeof(snNet_ctx))) {
        snErr_return(error, snErr_ErrMemory,
            "snNet_new: (*net) Failed to apply for memory.");
    }

    // 为结构体socket成员设定类型并将网络结构大小赋值
    // 如果即不是IPv4也不是IPv6就返回错误代码
    if(family == AF_INET) {
        (*net)->sockfdFamily = AF_INET;
        (*net)->addr_info_size = SN_NET_IPV4_ADDR_SIZE;
    } else if(family == AF_INET6) {
        (*net)->sockfdFamily = AF_INET6;
        (*net)->addr_info_size = SN_NET_IPV6_ADDR_SIZE;
    } else {
        snErr_return(error, snErr_NetFamily,
            "snNet_new: The type of socket must be AF_INET or AF_INET6.");
    }
    if(!snMemoryNew(SOCKADDR *, (*net)->addr_info, (*net)->addr_info_size)) {
        snErr_return(error, snErr_ErrMemory,
            "snNet_new: (*net)->addr_info Failed to apply for memory.");
    }

    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snErr_ctx) snNet_free SN_OPEN_API
SN_FUNC_OF((snNet_ctx **net))
{
    snErr_ctx error;
    if(!net) {
        snErr_return(error, snErr_ErrNULL, "snNet_free: net is NULL.");
    }

    if((*net)->addr_info) {
        snMemoryFree((*net)->addr_info);
    }
    if((*net)) {
        snMemoryFree((*net));
    }

    snErr_return(error, snErr_OK, "OK.");
}

// 初始化snNet对象（需提前申请内存空间）
SN_PUBLIC(snErr_ctx) snNet_init SN_OPEN_API
SN_FUNC_OF((snNet_ctx *net, snString hostname, sn_u16 port, snBool UDP))
{
    snErr_ctx error;

    if(!net) {
        snErr_return(error, snErr_ErrNULL, "snNet_init: net is NULL.");
    }
    if((net->sockfdFamily != AF_INET) && (net->sockfdFamily != AF_INET6)) {
        snErr_return(error, snErr_NetFamily,
            "snNet_init: The type of socket must be AF_INET or AF_INET6.");
    }

#   if defined(_WIN32)
    // 如果是Windows系统就初始化并启动WSADATA对象。
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws)) {
        snErr_return(error, snErr_NetWsaData, "snNet_init: WSADATA startup failed.");
    }
#   endif

    if(hostname) {
        SOCKADDR_IN *ipv4 = snNull; // 一个IPv4网络结构的指针
        SOCKADDR_IN6 *ipv6 = snNull; // 一个IPv6网络结构的指针

        if(UDP) {
            // 如果Socket是UDP类型
            net->sockfd = snNet_UDP_Socket(net->sockfdFamily, IPPROTO_IP);
            net->sockfdType = SN_NET_UDP_TYPE;
        } else {
            // 否则Socket是TCP类型
            net->sockfd = snNet_TCP_Socket(net->sockfdFamily, IPPROTO_IP);
            net->sockfdType = SN_NET_TCP_TYPE;
        }
        // 如果Socket申请失败就返回错误代码
        if(net->sockfd == snErr_Err32) {
            snErr_return(error, snErr_NetSocket,
                "snNet_init: Unable to create socket.");
        }

        // 解析域名，如果失败就返回错误代码
        error = snNet_resolveAddress(net, hostname);
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

    snErr_return(error, snErr_OK, "OK.");
}

// 域名解析函数
SN_PUBLIC(snErr_ctx) snNet_resolveAddress
SN_FUNC_OF((snNet_ctx *net, snString hostname))
{
    snErr_ctx error;

    if(!net || !hostname || !net->addr_info) {
        snErr_return(error, snErr_ErrNULL,
            "snNet_resolveAddress: net or hostname or net->addr_info is NULL.");
    }

    // 判断用户输入的网络家族是IPv4还是IPv6，如果都不是就返回错误代码
    if((net->sockfdFamily != AF_INET) && (net->sockfdFamily != AF_INET6)) {
        snErr_return(error, snErr_NetFamily,
            "snNet_resolveAddress: The type of socket must be AF_INET or AF_INET6.");
    }

    ADDRINFO *result = snNull;
    ADDRINFO hints = {.ai_family = net->sockfdFamily};

    // 进行域名解析，解析失败就返回错误代码
    if(getaddrinfo(hostname, "echo", &hints, &result)) {
        snErr_return(error, snErr_NetDomainResolv,
            "snNet_resolveAddress: Domain Name System failed.");
    }

    // 将解析结果保存至结构体成员中
    memcpy(net->addr_info, result->ai_addr, net->addr_info_size);

    freeaddrinfo(result); // 释放掉已结束使用的解析数据
    snErr_return(error, snErr_OK, "OK.");
}

// 设置发送与接收超时时间
SN_PUBLIC(snErr_ctx) snNet_timeout SN_OPEN_API
SN_FUNC_OF((snNet_ctx *net, snNetTime _user_TimeOut))
{
    snErr_ctx error;
    if(!net) {
        snErr_return(error, snErr_ErrNULL, "snNet_timeout: net is NULL.");
    }

    snNetSize optlen = sizeof(struct timeval);
    snNetTimer *optval = snNull;
    struct timeval _timeout;

    _timeout.tv_sec = floor(_user_TimeOut);
    _timeout.tv_usec = (long)(modf(_user_TimeOut, &_user_TimeOut) * 1000);

    if(_user_TimeOut) {
        optval = (snNetTimer *)&_timeout;
        if(setsockopt(net->sockfd, SOL_SOCKET, SO_SNDTIMEO, optval, optlen) ||
            setsockopt(net->sockfd, SOL_SOCKET, SO_RCVTIMEO, optval, optlen)) {
            snErr_return(error, snErr_NetSetSockOpt,
                "snNet_timeout: Error in setsockopt function.");
        }
    }

    snErr_return(error, snErr_OK, "OK.");
}

// 连接函数
SN_PUBLIC(snErr_ctx) snNet_connect SN_OPEN_API
SN_FUNC_OF((snNet_ctx *net))
{
    snErr_ctx error;
    if(!net) {
        snErr_return(error, snErr_ErrNULL, "snNet_connect: net is NULL.");
    }
    if(net->sockfdType == SN_NET_UDP_TYPE) {
        snErr_return(error, snErr_NetSockfdType,
            "snNet_connect: Wrong socket type, currently not expected to be UDP.");
    }

    // 连接至接收端，失败就返回错误代码
    if(connect(net->sockfd, net->addr_info, net->addr_info_size) == snErr_Err32) {
        snErr_return(error, snErr_NetConnect,
            "snNet_connect: Socket connection failed.");
    }

    snErr_return(error, snErr_OK, "OK.");
}

// 绑定函数
SN_PUBLIC(snErr_ctx) snNet_bind SN_OPEN_API
SN_FUNC_OF((snNet_ctx *net, snNetTime _user_TimeOut))
{
    snErr_ctx error;
    if(!net) {
        snErr_return(error, snErr_ErrNULL, "snNet_bind: net is NULL.");
    }

    snNetTimer *optval = snNull;
    struct timeval _timeout;

    if(_user_TimeOut) {
        _timeout.tv_sec = floor(_user_TimeOut);
        _timeout.tv_usec = (long)(modf(_user_TimeOut, &_user_TimeOut) * 1000);
        optval = (snNetTimer *)&_timeout;

        if(setsockopt(net->sockfd, SOL_SOCKET, SO_REUSEADDR, optval, sizeof(_timeout))) {
            snErr_return(error, snErr_NetSetSockOpt,
                "snNet_bind: Error in setsockopt function in snNet_timeout function.");
        }
    }

    // 如果绑定失败就返回错误代码
    if(bind(net->sockfd, net->addr_info, net->addr_info_size) == snErr_Err32) {
        snErr_return(error, snErr_NetBind, "snNet_bind: Socket binding failed.");
    }

    snErr_return(error, snErr_OK, "OK.");
}

// 监听函数
SN_PUBLIC(snErr_ctx) snNet_listen SN_OPEN_API
SN_FUNC_OF((snNet_ctx *net, sn_u32 _Listen))
{
    snErr_ctx error;
    if(!net) {
        snErr_return(error, snErr_ErrNULL, "snNet_listen: net is NULL.");
    }

    // 监听连接，失败或者Socket类型不正确就返回错误代码
    if(net->sockfdType == SN_NET_TCP_TYPE) {
        if(!_Listen) {
            _Listen = SN_NET_DEFAULT_LISTEN;
        }
        if(listen(net->sockfd, _Listen) == snErr_Err32) {
            snErr_return(error, snErr_NetListen,
                "snNet_listen: Socket listening failed.");
        }
    } else {
        snErr_return(error, snErr_NetSockfdType,
            "snNet_listen: Wrong socket type, currently not expected to be UDP.");
    }

    snErr_return(error, snErr_OK, "OK.");
}

// 等待连接函数
SN_PUBLIC(snErr_ctx) snNet_accept SN_OPEN_API
SN_FUNC_OF((snNet_ctx *dst, snNet_ctx *src))
{
    snErr_ctx error;
    if(!dst || !src) {
        snErr_return(error, snErr_ErrNULL, "snNet_accept: dst or src is NULL.");
    }
    if(!dst->addr_info || !src->addr_info) {
        snErr_return(error, snErr_ErrNULL,
            "snNet_accept: dst->addr_info or src->addr_info is NULL.");
    }

    // 接收连接，失败就返回错误代码
    if(src->sockfdType == SN_NET_TCP_TYPE) {
        dst->sockfd = accept(src->sockfd, dst->addr_info, &dst->addr_info_size);
        if(dst->sockfd == snErr_Err32) {
            snErr_return(error, snErr_NetAccept,
                "snNet_accept: The socket failed to accept the connection.");
        }
    } else {
        snErr_return(error, snErr_NetSockfdType,
            "snNet_accept: Wrong socket type, currently not expected to be UDP.");
    }

    snErr_return(error, snErr_OK, "OK.");
}

// 发送函数
SN_PUBLIC(snErr_ctx) snNet_send SN_OPEN_API
SN_FUNC_OF((snNet_ctx *net, snNetSize *_tSize, snNetBuf *buf, snNetSize size))
{
    snErr_ctx error;
    if(!net || !buf || !size) {
        snErr_return(error, snErr_ErrNULL,
            "snNet_send: net or buf or size is NULL.");
    }
    if(!_tSize) {
        snNetSize size;
        _tSize = &size;
    }

    // 判断Socket类型，TCP或UDP。发送数据。如果返回错误代码就返回对应的错误代码。
    switch(net->sockfdType) {
        case SN_NET_TCP_TYPE: // TCP
            *_tSize = send(net->sockfd, buf, size, 0);
            if(*_tSize == snErr_Err32) {
                snErr_return(error, snErr_NetSend,
                    "snNet_send: The socket failed to send data.");
            }
            break;
        case SN_NET_UDP_TYPE: // UDP
            *_tSize = sendto(net->sockfd, buf, size, 0,
                net->addr_info, net->addr_info_size);
            if(*_tSize == snErr_Err32) {
                snErr_return(error, snErr_NetSend,
                    "snNet_send: The socket failed to send data.");
            }
            break;
        default:
            snErr_return(error, snErr_NetSockfdType,
                "snNet_send: Wrong socket type, currently not expected to be UDP.");
    }

    snErr_return(error, snErr_OK, "OK.");
}

// 全部发送函数
SN_PUBLIC(snErr_ctx) snNet_sendall SN_OPEN_API
SN_FUNC_OF((snNet_ctx *net, snNetBuf *buf, snNetSize size))
{
    snErr_ctx error;
    if(!net || !buf || !size) {
        snErr_return(error, snErr_ErrNULL,
            "snNet_sendall: net or buf or size is NULL.");
    }
    snNetSize index, quotient, leftover, _tSize;

    // 计算需要发送SN_NET_BLOCKLEN大小的数据包的次数
    quotient = size / SN_NET_BLOCKLEN;
    // 计算是否还有剩下的数据包
    leftover = size % SN_NET_BLOCKLEN;

    for(index = 0; index < quotient; ++index) {
        // 如果发送错误就返回错误代码
        error = snNet_send(net, &_tSize, buf, SN_NET_BLOCKLEN);
        if(error.code) {
            return error;
        }
        buf += SN_NET_BLOCKLEN;
    }

    // 如果有剩下的数据包，那么就发送。如果发送错误就返回错误代码
    if(leftover) {
        error = snNet_send(net, &_tSize, buf, leftover);
        if(error.code) {
            return error;
        }
    }

    snErr_return(error, snErr_OK, "OK.");
}

// 接收函数
SN_PUBLIC(snErr_ctx) snNet_recv SN_OPEN_API
SN_FUNC_OF((snNet_ctx* net, snNetSize *_tSize, snNetBuf *buf, snNetSize size))
{
    snErr_ctx error;
    if(!net || !buf || !size) {
        snErr_return(error, snErr_ErrNULL,
            "snNet_recv: net or buf or size is NULL.");
    }
    if(!_tSize) {
        snNetSize size;
        _tSize = &size;
    }

    switch(net->sockfdType) {
        case SN_NET_TCP_TYPE:
            *_tSize = recv(net->sockfd, buf, size, 0);
            if(*_tSize == snErr_Err32) {
                snErr_return(error, snErr_NetRecv,
                    "snNet_recv: The socket failed to receive data.");
            }
            break;
        case SN_NET_UDP_TYPE:
            *_tSize = recvfrom(net->sockfd, buf, size, 0,
                net->addr_info, &net->addr_info_size);
            if(*_tSize == snErr_Err32) {
                snErr_return(error, snErr_NetRecv,
                    "snNet_recv: The socket failed to receive data.");
            }
            break;
        default:
            snErr_return(error, snErr_NetSockfdType,
                "snNet_recv: Wrong socket type, currently not expected to be UDP.");
    }

    snErr_return(error, snErr_OK, "OK.");
}

// 关闭套接字函数
SN_PUBLIC(snErr_ctx) snNet_close SN_OPEN_API
SN_FUNC_OF((snNet_ctx *net))
{
    snErr_ctx error;
    if(!net) {
        snErr_return(error, snErr_ErrNULL, "snNet_close: net is NULL.");
    }

    // 判断系统类型，根据对应系统使用不同的关闭函数将Socket与WSADATA进行关闭
    // 如果错误，就返回错误代码
#if defined(__linux)
    if((shutdown(net->sockfd, 2) | close(net->sockfd)) == snErr_Err32) {
        snErr_return(error, snErr_NetClose, "snNet_close: Socket close failed.");
    }
#elif defined(_WIN32)
    if((closesocket(net->sockfd) | WSACleanup()) == snErr_Err32) {
        snErr_return(error, snErr_NetClose, "snNet_close: Socket close failed.");
    }
#endif

    snErr_return(error, snErr_OK, "OK.");
}
