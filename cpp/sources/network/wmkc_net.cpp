#include <network/wmkc_net.hpp>

wmkcNet::IPEndPoint::IPEndPoint(std::string addr, wmkc_u16 port)
: addr(addr), port(port) {}

ADDRINFO *wmkcNet::getAddrInfo(wmkc_s32 family, wmkc_s32 type, wmkc_s32 proto, std::string addr, std::string serviceName)
{
    ADDRINFO hints = {0};
    hints.ai_family = family;
    hints.ai_socktype = type;
    hints.ai_protocol = proto;

    ADDRINFO *sockAddrRes = wmkcNull;
    wmkc_s32 err = getaddrinfo(addr.c_str(), serviceName.c_str(), &hints, &sockAddrRes);
    if(err) {
        wmkcNet::Socket_exception("wmkcNet::getAddrInfo");
    }
    return sockAddrRes;
}

std::string wmkcNet::networkAddr2stringAddr(wmkc_s32 family, const wmkcVoid *pAddr)
{
    char tmp_addr[INET6_ADDRSTRLEN];
    if(!inet_ntop(family, pAddr, tmp_addr, INET6_ADDRSTRLEN)) {
        wmkcNet::Socket_exception("wmkcNet::networkAddr2stringAddr");
    }
    return std::string(tmp_addr);
}

wmkc_u16 wmkcNet::networkPort2numberPort(const wmkc_u16 port)
{
    return ntohs(port);
}

wmkcNet::IPEndPoint wmkcNet::getNetworkInfo(wmkcNetSockT sockfd, wmkc_s32 family)
{
    SOCKADDR_IN *ipv4 = wmkcNull;
    SOCKADDR_IN6 *ipv6 = wmkcNull;
    SOCKADDR basicSockAddr = {0};
    socklen_t basicSockAddr_len = sizeof(basicSockAddr);
    wmkcNet::IPEndPoint addr_info;

    if(getsockname(sockfd, &basicSockAddr, &basicSockAddr_len) == WMKC_NET_ERROR) {
        wmkcNet::Socket_exception("wmkcNet::getNetworkInfo");
    }

    switch(family) {
        case AF_INET:
            ipv4 = (SOCKADDR_IN *)&basicSockAddr;
            addr_info.addr = wmkcNet::networkAddr2stringAddr(family, &ipv4->sin_addr);
            addr_info.port = wmkcNet::networkPort2numberPort(ipv4->sin_port);
            break;
        case AF_INET6:
            ipv6 = (SOCKADDR_IN6 *)&basicSockAddr;
            addr_info.addr = wmkcNet::networkAddr2stringAddr(family, &ipv6->sin6_addr);
            addr_info.port = wmkcNet::networkPort2numberPort(ipv6->sin6_port);
            break;
    }

    return addr_info;
}

wmkcNet::IPEndPoint wmkcNet::getNetworkInfo(wmkc_s32 family, wmkcVoid *pAddr)
{
    SOCKADDR_IN *ipv4 = wmkcNull;
    SOCKADDR_IN6 *ipv6 = wmkcNull;
    wmkcNet::IPEndPoint addr_info;
    
    switch(family) {
        case AF_INET:
            ipv4 = (SOCKADDR_IN *)pAddr;
            addr_info.addr = wmkcNet::networkAddr2stringAddr(family, &ipv4->sin_addr);
            addr_info.port = wmkcNet::networkPort2numberPort(ipv4->sin_port);
            break;
        case AF_INET6:
            ipv6 = (SOCKADDR_IN6 *)pAddr;
            addr_info.addr = wmkcNet::networkAddr2stringAddr(family, &ipv6->sin6_addr);
            addr_info.port = wmkcNet::networkPort2numberPort(ipv6->sin6_port);
            break;
    }

    return addr_info;
}

//****************************************************************************************************//
wmkcNet::Socket::Socket(wmkc_s32 _family, wmkc_s32 _type, wmkc_s32 _proto, wmkcNetSockT _fd)
: err(), timeout(), fd(), family(_family), type(_type), proto(_proto), transmissionLength(), lAddr(), rAddr()
{
    if(_fd == WMKC_NET_ERROR) {
        this->fd = socket(this->family, this->type, this->proto);
        if(this->fd == WMKC_NET_ERROR) {
            wmkcNet::Socket_exception("wmkcNet::Socket::Socket");
        }
    } else {
        this->fd = _fd;
    }
}

wmkcNet::Socket::~Socket()
{

}

void wmkcNet::Socket::settimeout(double _val)
{
    this->timeout = _val;
#   if defined(WMKC_PLATFORM_WINOS)
    DWORD _timeout = (DWORD)(this->timeout * 1000);
    wmkcChar *optval = (wmkcChar *)&_timeout;
#   elif defined(WMKC_PLATFORM_LINUX)
    double intpart = 0;
    double fracpart = modf(this->timeout, &intpart);
    struct timeval _timeout = {.tv_sec=(long)intpart, .tv_usec=(long)(fracpart * 1000000)};
    wmkcVoid *optval = (wmkcVoid *)&_timeout;
#   endif

    if(setsockopt(this->fd, SOL_SOCKET, SO_SNDTIMEO, optval, sizeof(_timeout)) ||
        setsockopt(this->fd, SOL_SOCKET, SO_RCVTIMEO, optval, sizeof(_timeout))) {
        wmkcNet::Socket_exception("wmkcNet::Socket::settimeout");
    }
}

void wmkcNet::Socket::connect(const std::string addr, const wmkc_u16 port)
{
    ADDRINFO *sockAddrResult = wmkcNet::getAddrInfo(this->family, this->type, this->proto,
        addr, std::to_string(port));

    this->err = ::connect(this->fd, sockAddrResult->ai_addr, sockAddrResult->ai_addrlen);
    if(this->err == WMKC_NET_ERROR) {
        wmkcNet::Socket_exception("wmkcNet::Socket::connect");
    }

    this->rAddr = wmkcNet::getNetworkInfo(this->family, sockAddrResult->ai_addr);
    this->lAddr = wmkcNet::getNetworkInfo(this->fd, this->family);
    freeaddrinfo(sockAddrResult);
}

void wmkcNet::Socket::bind(const std::string addr, const wmkc_u16 port)
{
    ADDRINFO *sockAddrResult = wmkcNet::getAddrInfo(this->family, this->type, this->proto,
        addr, std::to_string(port));
    this->err = ::bind(this->fd, sockAddrResult->ai_addr, sockAddrResult->ai_addrlen);
    if(this->err == WMKC_NET_ERROR) {
        wmkcNet::Socket_exception("wmkcNet::Socket::bind");
    }
    this->lAddr = wmkcNet::getNetworkInfo(this->family, sockAddrResult->ai_addr);
    freeaddrinfo(sockAddrResult);
}

void wmkcNet::Socket::listen(const wmkc_s32 backlog)
{
    this->err = ::listen(this->fd, backlog);
    if(this->err == WMKC_NET_ERROR) {
        wmkcNet::Socket_exception("wmkcNet::Socket::listen");
    }
}

wmkcNet::Socket wmkcNet::Socket::accept()
{
    SOCKADDR client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    wmkcNetSockT client_sockfd = ::accept(this->fd, &client_addr, &client_addr_len);
    if(client_sockfd == WMKC_NET_ERROR) {
        wmkcNet::Socket_exception("wmkcNet::Socket::accept");
    }
    return wmkcNet::Socket(this->family, this->type, this->proto, client_sockfd);
}

void wmkcNet::Socket::send(const std::string content, const wmkc_s32 flag)
{
    this->transmissionLength = ::send(this->fd, (wmkcNetBufT *)content.c_str(), content.size(), flag);
    if(this->transmissionLength == WMKC_NET_ERROR) {
        wmkcNet::Socket_exception("wmkcNet::Socket::send");
    }
}

void wmkcNet::Socket::sendall(const std::string content, const wmkc_s32 flag)
{
    char *offset_ptr = (char *)content.c_str();
    uint32_t size = (socklen_t)content.size();
    uint32_t retry_count = 5;

    while(size) {
        this->transmissionLength = ::send(this->fd, offset_ptr, size, flag);
        if(this->transmissionLength == WMKC_NET_ERROR && retry_count--) {
            // 将retry_count放入此块中，并判断是否为0，如果是，那么就返回一个false
            // 同时记得改变这些函数的返回类型为布尔型
            continue;
        }
        offset_ptr += this->transmissionLength;
        size -= this->transmissionLength;
    }
}

void wmkcNet::Socket::sendto(const std::string content, const wmkc_s32 flag)
{

}

std::string wmkcNet::Socket::recv(const wmkc_s32 len, const wmkc_s32 flag)
{
    wmkcNetBufT *_tmp = wmkcNull;
    if(!wmkcMem_new(wmkcNetBufT *, _tmp, len)) {
        wmkcErr_exception(wmkcErr_ErrMemory, "wmkcNet::Socket::recv",
            "Failed to allocate memory for _tmp.");
    }

    this->transmissionLength = ::recv(this->fd, _tmp, len, flag);
    if(this->transmissionLength == WMKC_NET_ERROR) {
        wmkcNet::Socket_exception("wmkcNet::Socket::recv");
    }
    std::string content((wmkcChar *)_tmp, this->transmissionLength);
    wmkcMem_free(_tmp);

    return content;
}

void wmkcNet::Socket::shutdown(const wmkc_s32 how)
{
    this->err = ::shutdown(this->fd, how);
    if(this->err == WMKC_NET_ERROR) {
        wmkcNet::Socket_exception("wmkcNet::Socket::shutdown");
    }
}

void wmkcNet::Socket::close()
{
#   if defined(WMKC_PLATFORM_WINOS)
    this->err = closesocket(this->fd);
#   elif defined(WMKC_PLATFORM_LINUX)
    this->err = ::close(this->fd);
#   endif
    if(this->err == WMKC_NET_ERROR) {
        wmkcNet::Socket_exception("wmkcNet::Socket::close");
    }
}
