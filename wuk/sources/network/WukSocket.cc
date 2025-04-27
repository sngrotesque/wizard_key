#include <network/WukSocket.hh>

wuk::net::Socket::Socket()
: _timeout(), _p_size(), _fd(), _family(), _type(), _proto()
{

}

wuk::net::Socket::Socket(wI32 family, wI32 type, wI32 proto, wSocket other_fd)
: _timeout(), _p_size(), _fd(), _family(family), _type(type), _proto(proto)
{
    if (static_cast<wI32>(other_fd) == WUK_NET_ERROR) {
        this->_fd = socket(this->_family, this->_type, this->_proto);
        if (static_cast<wI32>(this->_fd) == WUK_NET_ERROR) {
            throw wuk::net::Exception("wuk::net::Socket::Socket");
        }
    } else {
        this->_fd = other_fd;
    }
}

wuk::net::Socket::~Socket()
{
    
}

ADDRINFO *wuk::net::Socket::get_addr_info(std::string addr, std::string service_name) const
{
    ADDRINFO *result{nullptr};
    ADDRINFO hints{};

    hints.ai_family = this->_family;
    hints.ai_socktype = this->_type;
    hints.ai_protocol = this->_proto;

    wI32 wsaError = getaddrinfo(addr.c_str(), service_name.c_str(),
                                &hints, &result);
    if (wsaError) {
        throw wuk::net::Exception(wsaError, "wuk::net::IPEndPoint::get_addr_info");
    }

    return result;
}

void wuk::net::Socket::setsockopt(wI32 level, wI32 opt_name, wuk::net::SockOpt opt)
{
    wI32 err = ::setsockopt(this->_fd, level, opt_name,
                            static_cast<const char *>(opt.val), opt.val_len);
    if (err == WUK_NET_ERROR) {
        throw wuk::net::Exception("wuk::net::Socket::setsockopt");
    }
}

void wuk::net::Socket::getsockopt(wI32 level, wI32 opt_name, wuk::net::SockOpt &opt)
{
    wI32 err = ::getsockopt(this->_fd, level, opt_name, opt.val, &opt.val_len);
    if (err == WUK_NET_ERROR) {
        throw wuk::net::Exception("wuk::net::Socket::getsockopt");
    }
}

wuk::net::IPEndPoint wuk::net::Socket::getsockname(wSocket fd)
{
    SOCKADDR_STORAGE addr{};
    socklen_t addrlen{};

    SOCKADDR *addr_ptr = reinterpret_cast<SOCKADDR *>(&addr);

    wI32 err = ::getsockname(fd, addr_ptr, &addrlen);
    if (err == WUK_NET_ERROR) {
        throw wuk::net::Exception("wuk::net::Socket::getsockname");
    }

    return wuk::net::IPEndPoint{addr_ptr, addrlen};
}

wuk::net::IPEndPoint wuk::net::Socket::getsockname()
{
    return this->getsockname(this->_fd);
}

void wuk::net::Socket::settimeout(double timeout_val)
{
    this->_timeout = timeout_val;

#   if defined(WUK_PLATFORM_WINOS)
    DWORD _time_val = static_cast<DWORD>(this->_timeout * 1e3);
#   elif defined(WUK_PLATFORM_LINUX)
    double int_part{};
    double frac_part{modf(this->_timeout, &int_part)};
    struct timeval _time_val{
        static_cast<time_t>(int_part), static_cast<time_t>(frac_part * 1e6)
    };
#   endif
    wuk::net::SockOpt opt{reinterpret_cast<char *>(&_time_val), sizeof(_time_val)};

    this->setsockopt(SOL_SOCKET, SO_SNDTIMEO, opt);
    this->setsockopt(SOL_SOCKET, SO_RCVTIMEO, opt);
}

void wuk::net::Socket::connect(const std::string addr, const wU16 port)
{
    ADDRINFO *result = this->get_addr_info(addr, std::to_string(port));

    wI32 err = ::connect(this->_fd, result->ai_addr, result->ai_addrlen);
    if (err == WUK_NET_ERROR) {
        freeaddrinfo(result);
        throw wuk::net::Exception("wuk::net::Socket::connect");
    }

    // 未实现解析远程地址和当前地址，raddr，laddr。

    freeaddrinfo(result);
}

void wuk::net::Socket::bind(const std::string addr, const wU16 port)
{
    ADDRINFO *result = this->get_addr_info(addr, std::to_string(port));

    wI32 err = ::bind(this->_fd, result->ai_addr, result->ai_addrlen);
    if (err == WUK_NET_ERROR) {
        freeaddrinfo(result);
        throw wuk::net::Exception("wuk::net::Socket::bind");
    }

    // 未实现解析远程地址和当前地址，raddr，laddr。

    freeaddrinfo(result);
}

void wuk::net::Socket::listen(const wI32 backlog)
{
    if (::listen(this->_fd, backlog)) {
        throw wuk::net::Exception("wuk::net::Socket::listen");
    }
}

wuk::net::Socket wuk::net::Socket::accept()
{
    wuk::net::IPEndPoint info;

    wSocket other_fd = ::accept(this->_fd, info.get_ai_addr_ptr(),
                                info.get_ai_addrlen_ptr());

    // printf("wuk::net::Socket::accept TEST\n"
    //         "host_name: %s\nhost_port: %u\n",
    //         info.get_host().c_str(), info.get_port());

    if (static_cast<wI32>(other_fd) == WUK_NET_ERROR) {
        throw wuk::net::Exception("wuk::net::Socket::accept");
    }

    return wuk::net::Socket(this->_family, this->_type, this->_proto, other_fd);
}

void wuk::net::Socket::send(const char *buffer, socklen_t length, const wI32 flag)
{
    this->_p_size = ::send(this->_fd, buffer, length, flag);
    if (this->_p_size == WUK_NET_ERROR) {
        throw wuk::net::Exception("wuk::net::Socket::send");
    }
}

void wuk::net::Socket::sendall(const char *buffer, socklen_t length, const wI32 flag)
{
    wI32 retry_count{5};

    while (length) {
        try {
            this->send(buffer, length, flag);
        } catch (wuk::net::Exception &e) {
            if (retry_count) {
                retry_count--;
                continue;
            }
            throw wuk::net::Exception("wuk::net::Socket::sendall");
        }
        buffer += this->_p_size;
        length -= this->_p_size;
    }
}

void wuk::net::Socket::sendto(const char *buffer, socklen_t length,
                            wuk::net::IPEndPoint &target, const wI32 flag)
{
    this->_p_size = ::sendto(this->_fd, buffer, length, flag,
                            target.get_ai_addr(), target.get_ai_addrlen());
    if (this->_p_size == WUK_NET_ERROR) {
        throw wuk::net::Exception("wuk::net::Socket::sendto");
    }
}

void wuk::net::Socket::send(const wuk::Buffer buffer, const wI32 flag)
{
    this->send(buffer.get_cstr(), buffer.get_length(), flag);
}

void wuk::net::Socket::sendall(const wuk::Buffer buffer, const wI32 flag)
{
    this->sendall(buffer.get_cstr(), buffer.get_length(), flag);
}

void wuk::net::Socket::sendto(const wuk::Buffer buffer, wuk::net::IPEndPoint &target, const wI32 flag)
{
    this->sendto(buffer.get_cstr(), buffer.get_length(), target, flag);
}

void wuk::net::Socket::send(const std::string buffer, const wI32 flag)
{
    this->send(buffer.c_str(), buffer.length(), flag);
}

void wuk::net::Socket::sendall(const std::string buffer, const wI32 flag)
{
    this->sendall(buffer.c_str(), buffer.length(), flag);
}

void wuk::net::Socket::sendto(const std::string buffer, wuk::net::IPEndPoint &target, const wI32 flag)
{
    this->sendto(buffer.c_str(), buffer.length(), target, flag);
}

wuk::Buffer wuk::net::Socket::recv(const wI32 length, const wI32 flag)
{
    wuk::Buffer buffer{static_cast<wSize>(length)};

    this->_p_size = ::recv(this->_fd,
                        reinterpret_cast<char *>(buffer.append_write(length)),
                        length, flag);
    
    if (this->_p_size == WUK_NET_ERROR) {
        throw wuk::net::Exception("wuk::net::Socket::recv");
    }

    return buffer;
}

wuk::Buffer wuk::net::Socket::recvfrom(const wI32 length, wuk::net::IPEndPoint &from, const wI32 flag)
{
    // 未完成
    wuk::Buffer buffer{static_cast<wSize>(length)};

    SOCKADDR_STORAGE ai_addr{};
    socklen_t ai_addrlen{sizeof(ai_addr)};

    SOCKADDR *ai_addr_ptr = reinterpret_cast<SOCKADDR *>(&ai_addr);

    this->_p_size = ::recvfrom(this->_fd, reinterpret_cast<char *>(buffer.append_write(length)),
                                length, flag, ai_addr_ptr, &ai_addrlen);

    from = wuk::net::IPEndPoint{ai_addr_ptr, ai_addrlen};

    if (this->_p_size == WUK_NET_ERROR) {
        throw wuk::net::Exception("wuk::net::Socket::recvfrom");
    }

    return buffer;
}

void wuk::net::Socket::shutdown(const wI32 how)
{
    if (::shutdown(this->_fd, how) == WUK_NET_ERROR) {
        throw wuk::net::Exception("wuk::net::Socket::shutdown");
    }
}

void wuk::net::Socket::shutdown(wuk::net::SD_SW how)
{
    this->shutdown(static_cast<wI32>(how));
}

void wuk::net::Socket::close()
{
#               if defined(WUK_PLATFORM_WINOS)
    if (::closesocket(this->_fd) == WUK_NET_ERROR)
#               elif defined(WUK_PLATFORM_LINUX)
    if (::close(this->_fd) == WUK_NET_ERROR)
#               endif
    {
        throw wuk::net::Exception("wuk::net::Socket::close");
    }
}

wSocket wuk::net::Socket::get_fileno() const
{
    return this->_fd;
}

wI32 wuk::net::Socket::get_transmission_length() const
{
    return this->_p_size;
}
