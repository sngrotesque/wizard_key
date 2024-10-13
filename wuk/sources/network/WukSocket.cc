#include <network/WukSocket.hh>

//** wuk::net::IPEndPoint *********************************************************//
wuk::net::IPEndPoint::IPEndPoint(std::string addr, wU16 port)
: addr(addr), port(port)
{

}

wuk::net::IPEndPoint::IPEndPoint()
: addr(), port()
{

}

//** wuk::net::SocketOption *******************************************************//
wuk::net::SocketOption::SocketOption(const void *val, socklen_t val_len)
: val(val), val_len(val_len)
{
    if(!this->val) {
        throw wuk::Exception(wukErr_ErrNULL, "wuk::net::SocketOption::SocketOption",
            "val is nullptr.");
    }
}

wuk::net::SocketOption::SocketOption(socklen_t val)
: val(&val), val_len(sizeof(val))
{

}

wuk::net::SocketOption::SocketOption(std::string val)
: val(val.c_str()), val_len(val.size())
{

}

//** wuk::net::Socket *************************************************************//
ADDRINFO *wuk::net::Socket::get_addr_info(wS32 family, wS32 type, wS32 proto,
                                std::string addr, std::string serviceName)
{
    ADDRINFO *sockAddrRes = nullptr;
    ADDRINFO hints = {0};
    hints.ai_family = family;
    hints.ai_socktype = type;
    hints.ai_protocol = proto;

    if(getaddrinfo(addr.c_str(), serviceName.c_str(), &hints, &sockAddrRes)) {
        wuk::net::exception("wuk::net::Socket::get_addr_info");
    }
    return sockAddrRes;
}

std::string wuk::net::Socket::network_addr_to_string_addr(wS32 family, const void *pAddr)
{
    char tmp_addr[INET6_ADDRSTRLEN];
    if(!inet_ntop(family, pAddr, tmp_addr, INET6_ADDRSTRLEN)) {
        wuk::net::exception("wuk::net::Socket::network_addr_to_string_addr");
    }
    return std::string(tmp_addr);
}

wU16 wuk::net::Socket::network_port_to_number_port(const wU16 port)
{
    return ntohs(port);
}

wuk::net::IPEndPoint wuk::net::Socket::get_network_info(wSocket sockfd, wS32 family)
{
    SOCKADDR_IN *ipv4 = nullptr;
    SOCKADDR_IN6 *ipv6 = nullptr;
    SOCKADDR basicSockAddr{0};
    socklen_t basicSockAddr_len{sizeof(basicSockAddr)};
    wuk::net::IPEndPoint addr_info;

    if(getsockname(sockfd, &basicSockAddr, &basicSockAddr_len) == WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::Socket::get_network_info");
    }

    switch(family) {
    case AF_INET:
        ipv4 = reinterpret_cast<SOCKADDR_IN *>(&basicSockAddr);
        addr_info.addr = this->network_addr_to_string_addr(family,
                                                        &ipv4->sin_addr);
        addr_info.port = this->network_port_to_number_port(ipv4->sin_port);
        break;
    case AF_INET6:
        ipv6 = reinterpret_cast<SOCKADDR_IN6 *>(&basicSockAddr);
        addr_info.addr = this->network_addr_to_string_addr(family,
                                                        &ipv6->sin6_addr);
        addr_info.port = this->network_port_to_number_port(ipv6->sin6_port);
        break;
    }

    return addr_info;
}

wuk::net::IPEndPoint wuk::net::Socket::get_network_info(wS32 family, SOCKADDR *pAddr)
{
    SOCKADDR_IN *ipv4 = nullptr;
    SOCKADDR_IN6 *ipv6 = nullptr;
    wuk::net::IPEndPoint addr_info;
    
    switch(family) {
    case AF_INET:
        ipv4 = reinterpret_cast<SOCKADDR_IN *>(pAddr);
        addr_info.addr = this->network_addr_to_string_addr(family,
                                                        &ipv4->sin_addr);
        addr_info.port = this->network_port_to_number_port(ipv4->sin_port);
        break;
    case AF_INET6:
        ipv6 = reinterpret_cast<SOCKADDR_IN6 *>(pAddr);
        addr_info.addr = this->network_addr_to_string_addr(family,
                                                        &ipv6->sin6_addr);
        addr_info.port = this->network_port_to_number_port(ipv6->sin6_port);
        break;
    }

    return addr_info;
}

// Socket::Socket
wuk::net::Socket::Socket(wS32 _family, wS32 _type, wS32 _proto, wSocket _fd)
: timeout(), fd(), family(_family), type(_type), proto(_proto),t_size(),
lAddr(), rAddr()
{
    if(static_cast<wI32>(_fd) == WUK_NET_ERROR) {
        this->fd = socket(this->family, this->type, this->proto);
        if(static_cast<wI32>(this->fd) == WUK_NET_ERROR) {
            wuk::net::exception("wuk::net::Socket::Socket");
        }
    } else {
        this->fd = _fd;
    }
}

wuk::net::Socket::Socket()
: timeout(), fd(), family(AF_INET), type(SOCK_STREAM), proto(IPPROTO_TCP),t_size(),
lAddr(), rAddr()
{

}

wuk::net::Socket::~Socket()
{
    // this->close();
}

void wuk::net::Socket::setsockopt(int level, int optName, SocketOption opt)
{
    wS32 err = ::setsockopt(this->fd, level, optName, (char *)opt.val, opt.val_len);
    if(err == WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::Socket::setsockopt");
    }
}

void wuk::net::Socket::getsockopt(int level, int optName, SocketOption opt)
{
    wS32 err = ::getsockopt(this->fd, level, optName, (char *)opt.val, &opt.val_len);
    if(err == WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::Socket::getsockopt");
    }
}

void wuk::net::Socket::settimeout(double _val)
{
    this->timeout = _val;
#   if defined(WUK_PLATFORM_WINOS)
    DWORD _timeout = static_cast<DWORD>(this->timeout * 1e3);
#   elif defined(WUK_PLATFORM_LINUX)
    double intpart = 0;
    double fracpart = modf(this->timeout, &intpart);
    struct timeval _timeout = {
        .tv_sec  = static_cast<time_t>(intpart),
        .tv_usec = static_cast<time_t>(fracpart * 1e6)
    };
#   endif
    char *optval = reinterpret_cast<char *>(&_timeout);

    if(::setsockopt(this->fd, SOL_SOCKET, SO_SNDTIMEO, optval, sizeof(_timeout)) ||
        ::setsockopt(this->fd, SOL_SOCKET, SO_RCVTIMEO, optval, sizeof(_timeout))) {
        wuk::net::exception("wuk::net::Socket::settimeout");
    }
}

void wuk::net::Socket::connect(const std::string addr, const wU16 port)
{
    ADDRINFO *sAddrRes = this->get_addr_info(
        this->family, this->type, this->proto, addr, std::to_string(port));

    if(::connect(this->fd, sAddrRes->ai_addr, sAddrRes->ai_addrlen) == WUK_NET_ERROR) {
        freeaddrinfo(sAddrRes);
        wuk::net::exception("wuk::net::Socket::connect");
    }

    this->rAddr = this->get_network_info(this->family, sAddrRes->ai_addr);
    this->lAddr = this->get_network_info(this->fd, this->family);

    freeaddrinfo(sAddrRes);
}

void wuk::net::Socket::bind(const std::string addr, const wU16 port)
{
    ADDRINFO *sAddrRes = this->get_addr_info(
        this->family, this->type, this->proto, addr, std::to_string(port));

    if(::bind(this->fd, sAddrRes->ai_addr, sAddrRes->ai_addrlen) == WUK_NET_ERROR) {
        freeaddrinfo(sAddrRes);
        wuk::net::exception("wuk::net::Socket::bind");
    }

    this->lAddr = this->get_network_info(this->family, sAddrRes->ai_addr);

    freeaddrinfo(sAddrRes);
}

void wuk::net::Socket::listen(const wS32 backlog)
{
    if(::listen(this->fd, backlog) == WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::Socket::listen");
    }
}

wuk::net::Socket wuk::net::Socket::accept()
{
    SOCKADDR cAddr;
    socklen_t cAddr_len = sizeof(cAddr);
    wSocket cSockfd;

    cSockfd = ::accept(this->fd, &cAddr, &cAddr_len);
    if(static_cast<wI32>(cSockfd) == WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::Socket::accept");
    }

    return wuk::net::Socket(this->family, this->type, this->proto, cSockfd);
}

void wuk::net::Socket::send(const std::string content, const wS32 flag)
{
    this->t_size = ::send(this->fd, (char *)content.c_str(),
                                    content.size(), flag);

    if(this->t_size == WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::Socket::send");
    }
}

void wuk::net::Socket::sendall(const std::string content, const wS32 flag)
{
    const char *offset_ptr = content.c_str();
    wU32 size = static_cast<wU32>(content.size());
    wU32 retry_count{5};

    while(size) {
        this->t_size = ::send(this->fd, offset_ptr, size, flag);

        if((this->t_size == WUK_NET_ERROR)) {
            if(retry_count) {
                retry_count--;
                continue;
            } else {
                wuk::net::exception("wuk::net::Socket::sendall");
            }
        }

        offset_ptr += this->t_size;
        size       -= this->t_size;
    }
}

std::string wuk::net::Socket::recv(const wS32 len, const wS32 flag)
{
    char *buffer = new (std::nothrow) char[len];
    if(!buffer) {
        throw wuk::Exception(wukErr_ErrMemory, "wuk::net::Socket::recv",
            "Failed to allocate memory for buffer.");
    }

    this->t_size = ::recv(this->fd, buffer, len, flag);
    if(this->t_size == WUK_NET_ERROR) {
        delete[] buffer;
        wuk::net::exception("wuk::net::Socket::recv");
    }

    std::string content{buffer, static_cast<wSize>(this->t_size)};

    delete[] buffer;
    return content;
}

void wuk::net::Socket::sendto(const std::string content, wuk::net::IPEndPoint target,
                            const wS32 flag)
{
    ADDRINFO *sAddrRes = this->get_addr_info(this->family, this->type, this->proto,
                                            target.addr, std::to_string(target.port));

    this->t_size = ::sendto(this->fd, content.c_str(), content.size(),
                                    flag, sAddrRes->ai_addr, sAddrRes->ai_addrlen);
    if(this->t_size == WUK_NET_ERROR) {
        freeaddrinfo(sAddrRes);
        wuk::net::exception("wuk::net::Socket::send");
    }

    freeaddrinfo(sAddrRes);
}

std::string wuk::net::Socket::recvfrom(const wS32 len, SOCKADDR *from,
                                        socklen_t *fromlen, const wS32 flag)
{
    char *buffer = new (std::nothrow) char[len];
    if(!buffer) {
        throw wuk::Exception(wukErr_ErrMemory, "wuk::net::Socket::recvfrom",
            "Failed to allocate memory for buffer.");
    }

    this->t_size = ::recvfrom(this->fd, buffer, len, flag, from, fromlen);
    if(this->t_size == WUK_NET_ERROR) {
        delete[] buffer;
        exception("wuk::net::Socket::recvfrom");
    }

    std::string content(buffer, this->t_size);

    delete[] buffer;
    return content;
}

void wuk::net::Socket::shutdown(wuk::net::SD_SW how)
{
    this->shutdown(static_cast<wI32>(how));
}

void wuk::net::Socket::shutdown(const wS32 how)
{
    if(::shutdown(this->fd, how) == WUK_NET_ERROR) {
        wuk::net::exception("wuk::net::Socket::shutdown");
    }
}

void wuk::net::Socket::close()
{
#   if defined(WUK_PLATFORM_WINOS)
    if(::closesocket(this->fd) == WUK_NET_ERROR)
#   elif defined(WUK_PLATFORM_LINUX)
    if(::close(this->fd) == WUK_NET_ERROR)
#   endif
    {
        wuk::net::exception("wuk::net::Socket::close");
    }
}
