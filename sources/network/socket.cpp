#include <network/socket.hpp>

ADDRINFO *wmkc::net::getAddrInfo(wS32 family, wS32 type, wS32 proto,
                                std::string addr, std::string serviceName)
{
    ADDRINFO *sockAddrRes = nullptr;
    ADDRINFO hints = {0};
    hints.ai_family = family;
    hints.ai_socktype = type;
    hints.ai_protocol = proto;

    if(getaddrinfo(addr.c_str(), serviceName.c_str(), &hints, &sockAddrRes)) {
        wmkc::net::exception("wmkc::net::getAddrInfo");
    }
    return sockAddrRes;
}

std::string wmkc::net::networkAddr2stringAddr(wS32 family, const wVoid *pAddr)
{
    char tmp_addr[INET6_ADDRSTRLEN];
    if(!inet_ntop(family, pAddr, tmp_addr, INET6_ADDRSTRLEN)) {
        wmkc::net::exception("wmkc::net::networkAddr2stringAddr");
    }
    return std::string(tmp_addr);
}

wU16 wmkc::net::networkPort2numberPort(const wU16 port)
{
    return ntohs(port);
}

wmkc::net::IPEndPoint wmkc::net::getNetworkInfo(wSocket sockfd, wS32 family)
{
    SOCKADDR_IN *ipv4 = nullptr;
    SOCKADDR_IN6 *ipv6 = nullptr;
    SOCKADDR basicSockAddr = {0};
    socklen_t basicSockAddr_len = sizeof(basicSockAddr);
    wmkc::net::IPEndPoint addr_info;

    if(getsockname(sockfd, &basicSockAddr, &basicSockAddr_len) == WMKC_NET_ERROR) {
        wmkc::net::exception("wmkc::net::getNetworkInfo");
    }

    switch(family) {
        case AF_INET:
            ipv4 = (SOCKADDR_IN *)&basicSockAddr;
            addr_info.addr = wmkc::net::networkAddr2stringAddr(family,
                                                            &ipv4->sin_addr);
            addr_info.port = wmkc::net::networkPort2numberPort(ipv4->sin_port);
            break;
        case AF_INET6:
            ipv6 = (SOCKADDR_IN6 *)&basicSockAddr;
            addr_info.addr = wmkc::net::networkAddr2stringAddr(family,
                                                            &ipv6->sin6_addr);
            addr_info.port = wmkc::net::networkPort2numberPort(ipv6->sin6_port);
            break;
    }

    return addr_info;
}

wmkc::net::IPEndPoint wmkc::net::getNetworkInfo(wS32 family, SOCKADDR *pAddr)
{
    SOCKADDR_IN *ipv4 = nullptr;
    SOCKADDR_IN6 *ipv6 = nullptr;
    wmkc::net::IPEndPoint addr_info;
    
    switch(family) {
        case AF_INET:
            ipv4 = (SOCKADDR_IN *)pAddr;
            addr_info.addr = wmkc::net::networkAddr2stringAddr(family,
                                                            &ipv4->sin_addr);
            addr_info.port = wmkc::net::networkPort2numberPort(ipv4->sin_port);
            break;
        case AF_INET6:
            ipv6 = (SOCKADDR_IN6 *)pAddr;
            addr_info.addr = wmkc::net::networkAddr2stringAddr(family,
                                                            &ipv6->sin6_addr);
            addr_info.port = wmkc::net::networkPort2numberPort(ipv6->sin6_port);
            break;
    }

    return addr_info;
}

//*********************************************************************************//
wmkc::net::Socket::Socket(wS32 _family, wS32 _type, wS32 _proto, wSocket _fd)
: timeout(), fd(), family(_family), type(_type), proto(_proto),transmissionLength(),
lAddr(), rAddr()
{
    if(_fd == WMKC_NET_ERROR) {
        this->fd = socket(this->family, this->type, this->proto);
        if(this->fd == WMKC_NET_ERROR) {
            wmkc::net::exception("wmkc::net::Socket::Socket");
        }
    } else {
        this->fd = _fd;
    }
}

wmkc::net::Socket::~Socket()
{
    // this->close();
}

void wmkc::net::Socket::settimeout(double _val)
{
    this->timeout = _val;
#   if defined(WMKC_PLATFORM_WINOS)
    DWORD _timeout = (DWORD)(this->timeout * 1e3);
#   elif defined(WMKC_PLATFORM_LINUX)
    double intpart = 0;
    double fracpart = modf(this->timeout, &intpart);
    struct timeval _timeout = {.tv_sec=(long)intpart, .tv_usec=(long)(fracpart * 1e6)};
#   endif
    wChar *optval = (wChar *)&_timeout;

    if(setsockopt(this->fd, SOL_SOCKET, SO_SNDTIMEO, optval, sizeof(_timeout)) ||
        setsockopt(this->fd, SOL_SOCKET, SO_RCVTIMEO, optval, sizeof(_timeout))) {
        wmkc::net::exception("wmkc::net::Socket::settimeout");
    }
}

void wmkc::net::Socket::connect(const std::string addr, const wU16 port)
{
    ADDRINFO *sAddrRes = wmkc::net::getAddrInfo(
        this->family, this->type, this->proto, addr, std::to_string(port));

    if(::connect(this->fd, sAddrRes->ai_addr, sAddrRes->ai_addrlen) == WMKC_NET_ERROR) {
        freeaddrinfo(sAddrRes);
        wmkc::net::exception("wmkc::net::Socket::connect");
    }

    this->rAddr = wmkc::net::getNetworkInfo(this->family, sAddrRes->ai_addr);
    this->lAddr = wmkc::net::getNetworkInfo(this->fd, this->family);
    freeaddrinfo(sAddrRes);
}

void wmkc::net::Socket::bind(const std::string addr, const wU16 port)
{
    ADDRINFO *sAddrRes = wmkc::net::getAddrInfo(this->family, this->type, this->proto,
                                                addr, std::to_string(port));

    if(::bind(this->fd, sAddrRes->ai_addr, sAddrRes->ai_addrlen) == WMKC_NET_ERROR) {
        freeaddrinfo(sAddrRes);
        wmkc::net::exception("wmkc::net::Socket::bind");
    }
    this->lAddr = wmkc::net::getNetworkInfo(this->family, sAddrRes->ai_addr);

    freeaddrinfo(sAddrRes);
}

void wmkc::net::Socket::listen(const wS32 backlog)
{
    if(::listen(this->fd, backlog) == WMKC_NET_ERROR) {
        wmkc::net::exception("wmkc::net::Socket::listen");
    }
}

wmkc::net::Socket wmkc::net::Socket::accept()
{
    SOCKADDR cAddr;
    socklen_t cAddr_len = sizeof(cAddr);
    wSocket cSockfd;

    if((cSockfd = ::accept(this->fd, &cAddr, &cAddr_len)) == WMKC_NET_ERROR) {
        wmkc::net::exception("wmkc::net::Socket::accept");
    }

    return wmkc::net::Socket(this->family, this->type, this->proto, cSockfd);
}

void wmkc::net::Socket::send(const std::string content, const wS32 flag)
{
    this->transmissionLength = ::send(this->fd, (wChar *)content.c_str(),
                                                    content.size(), flag);

    if(this->transmissionLength == WMKC_NET_ERROR) {
        wmkc::net::exception("wmkc::net::Socket::send");
    }
}

void wmkc::net::Socket::sendall(const std::string content, const wS32 flag)
{
    wChar *offset_ptr = (wChar *)content.c_str();
    wU32 size = (socklen_t)content.size();
    wU32 retry_count = 5;

    while(size) {
        this->transmissionLength = ::send(this->fd, offset_ptr, size, flag);

        if((this->transmissionLength == WMKC_NET_ERROR)) {
            if(retry_count) {
                retry_count--;
                continue;
            } else {
                wmkc::net::exception("wmkc::net::Socket::sendall");
            }
        }

        offset_ptr += this->transmissionLength;
        size       -= this->transmissionLength;
    }
}

std::string wmkc::net::Socket::recv(const wS32 len, const wS32 flag)
{
    wChar *buffer = new wChar[len];
    if(!buffer) {
        wmkc::exception(wmkcErr_ErrMemory, "wmkc::net::Socket::recv",
            "Failed to allocate memory for buffer.");
    }

    this->transmissionLength = ::recv(this->fd, buffer, len, flag);
    if(this->transmissionLength == WMKC_NET_ERROR) {
        delete[] buffer;
        wmkc::net::exception("wmkc::net::Socket::recv");
    }

    std::string content((wChar *)buffer, this->transmissionLength);

    delete[] buffer;
    return content;
}

void wmkc::net::Socket::sendto(const std::string content, wmkc::net::IPEndPoint target,
                            const wS32 flag)
{
    ADDRINFO *sAddrRes = wmkc::net::getAddrInfo(this->family, this->type, this->proto,
                                            target.addr, std::to_string(target.port));

    this->transmissionLength = ::sendto(this->fd, content.c_str(), content.size(),
                                    flag, sAddrRes->ai_addr, sAddrRes->ai_addrlen);
    if(this->transmissionLength == WMKC_NET_ERROR) {
        freeaddrinfo(sAddrRes);
        wmkc::net::exception("wmkc::net::Socket::send");
    }

    freeaddrinfo(sAddrRes);
}

std::string wmkc::net::Socket::recvfrom(const wS32 len, SOCKADDR *from,
                                        socklen_t *fromlen, const wS32 flag)
{
    wChar *buffer = new wChar[len];
    if(!buffer) {
        wmkc::exception(wmkcErr_ErrMemory, "wmkc::net::Socket::recvfrom",
            "Failed to allocate memory for buffer.");
    }

    this->transmissionLength = ::recvfrom(this->fd, buffer, len, flag, from, fromlen);
    if(this->transmissionLength == WMKC_NET_ERROR) {
        delete[] buffer;
        exception("wmkc::net::Socket::recvfrom");
    }

    std::string content((wChar *)buffer, this->transmissionLength);

    delete[] buffer;
    return content;
}

void wmkc::net::Socket::shutdown(const wS32 how)
{
    if(::shutdown(this->fd, how) == WMKC_NET_ERROR) {
        wmkc::net::exception("wmkc::net::Socket::shutdown");
    }
}

void wmkc::net::Socket::close()
{
#   if defined(WMKC_PLATFORM_WINOS)
    if(::closesocket(this->fd) == WMKC_NET_ERROR)
#   elif defined(WMKC_PLATFORM_LINUX)
    if(::close(this->fd) == WMKC_NET_ERROR)
#   endif
    {
        wmkc::net::exception("wmkc::net::Socket::close");
    }
}
