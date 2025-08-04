#include <net/WukSocket.hh>

// WukAddrinfo BEGIN

wuk::net::WukAddrinfo::WukAddrinfo(wI32 family, wI32 sock_type, wI32 proto)
{
    this->hints.ai_family = family;
    this->hints.ai_socktype = sock_type;
    this->hints.ai_protocol = proto;
}

wuk::net::WukAddrinfo::~WukAddrinfo()
{
    freeaddrinfo(res);
}

void wuk::net::WukAddrinfo::resolve(const std::string &addr, const wU16 &port)
{
    wI32 err = getaddrinfo(addr.c_str(), std::to_string(port).c_str(),
            &this->hints, &this->res);
    if (err) {
        throw wuk::Exception(err, "WukAddrinfo::resolve",
            wuk::net::SystemError::message(err).c_str());
    }
}

const sockaddr *wuk::net::WukAddrinfo::get_addr() const
{
    if (!this->res) {
        throw wuk::Exception(wuk::Error::NPTR, "WukAddrinfo::get_addr",
            "this->res is nullptr.");
    }
    return this->res->ai_addr;
}

socklen_t wuk::net::WukAddrinfo::get_addrlen() const
{
    if (!this->res) {
        throw wuk::Exception(wuk::Error::NPTR, "WukAddrinfo::get_addrlen",
            "this->res is nullptr.");
    }
    return this->res->ai_addrlen;
}

// WukSockaddr BEGIN

wuk::net::WukSockaddr::WukSockaddr(const sockaddr_storage *addr, const socklen_t &addrlen)
{
    this->set_addr(reinterpret_cast<const sockaddr *>(addr), addrlen);
}

wuk::net::WukSockaddr::WukSockaddr(const sockaddr *addr, const socklen_t &addrlen)
{
    this->set_addr(addr, addrlen);
}

sockaddr *wuk::net::WukSockaddr::set_addr() noexcept
{
    return reinterpret_cast<sockaddr *>(&this->addr);
}

socklen_t *wuk::net::WukSockaddr::set_addrlen() noexcept
{
    return &this->addrlen;
}

void wuk::net::WukSockaddr::set_addr(const sockaddr *addr, const socklen_t &addrlen)
{
    if (!addr || addrlen > static_cast<socklen_t>(sizeof(sockaddr_storage))) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::net::WukSockaddr::set_addr",
            "Invalid address or length");
    }
    memcpy(&this->addr, addr, addrlen);
}

void wuk::net::WukSockaddr::set_addr(const WukSockaddr &addr)
{
    this->set_addr(addr.get_addr(), addr.get_addrlen());
}

const sockaddr *wuk::net::WukSockaddr::get_addr() const noexcept
{
    return reinterpret_cast<const sockaddr *>(&this->addr);
}

socklen_t wuk::net::WukSockaddr::get_addrlen() const noexcept
{
    return this->addrlen;
}

// WukSocket BEGIN

wuk::net::WukSocket::WukSocket(wI32 family, wI32 sock_type, wI32 proto)
: m_family(family), m_sock_type(sock_type), m_proto(proto)
{
    this->fd = socket(family, sock_type, proto);
    if (static_cast<wI32>(this->fd) == NETERROR) {
        wI32 err_code = wuk::net::SystemError::code();
        throw wuk::Exception(err_code, "wuk::net::WukSocket::WukSocket",
            wuk::net::SystemError::message(err_code).c_str());
    }
}

wuk::net::WukSocket::WukSocket(wI32 family, wI32 sock_type, wI32 proto, wSocket cur_fd)
: m_family(family), m_sock_type(sock_type), m_proto(proto)
{
    this->fd = cur_fd;
}

const wuk::net::WukSockaddr wuk::net::WukSocket::getsockname()
{
    wuk::net::WukSockaddr addr;
    wI32 err = ::getsockname(this->fd, addr.set_addr(), addr.set_addrlen());
    if (err == NETERROR) {
        wI32 err_code = wuk::net::SystemError::code();
        throw wuk::Exception(err_code, "wuk::net::WukSocket::getsockname",
            wuk::net::SystemError::message(err_code).c_str());
    }
    return addr;
}

void wuk::net::WukSocket::connect(const std::string &addr, const wU16 &port)
{
    WukAddrinfo info(this->m_family);
    info.resolve(addr, port);
    wI32 err = ::connect(this->fd, info.get_addr(), info.get_addrlen());
    if (err == NETERROR) {
        wI32 err_code = wuk::net::SystemError::code();
        throw wuk::Exception(err_code, "wuk::net::WukSocket::connect",
            wuk::net::SystemError::message(err_code).c_str());
    }
    this->m_raddr.set_addr(info.get_addr(), info.get_addrlen());
    this->m_laddr.set_addr(this->getsockname());
}

void wuk::net::WukSocket::bind(const std::string &addr, const wU16 &port)
{
    WukAddrinfo info(this->m_family);
    info.resolve(addr, port);
    wI32 err = ::bind(this->fd, info.get_addr(), info.get_addrlen());
    if (err == NETERROR) {
        wI32 err_code = wuk::net::SystemError::code();
        throw wuk::Exception(err_code, "wuk::net::WukSocket::bind",
            wuk::net::SystemError::message(err_code).c_str());
    }
    this->m_laddr.set_addr(info.get_addr(), info.get_addrlen());
}

std::optional<wuk::net::WukSocket> wuk::net::WukSocket::accept() const
{
    sockaddr_storage client_addrinfo {0};
    socklen_t client_addrinfo_len = sizeof(client_addrinfo);

    wSocket client_sock = ::accept(this->fd,
                                reinterpret_cast<sockaddr *>(&client_addrinfo),
                                &client_addrinfo_len);
    if (static_cast<wI32>(client_sock) == NETERROR) {
        wI32 err_code = wuk::net::SystemError::code();
        throw wuk::Exception(err_code, "wuk::net::WukSocket::accept",
            wuk::net::SystemError::message(err_code).c_str());
    }
    WukSocket new_sock(this->m_family, this->m_sock_type, this->m_proto, client_sock);
    new_sock.set_raddr(WukSockaddr(&client_addrinfo, client_addrinfo_len));
    new_sock.set_laddr(new_sock.getsockname());

    return new_sock;
}

void wuk::net::WukSocket::listen(const wU32 &backlog)
{
    wI32 err = ::listen(this->fd, backlog);
    if (err == NETERROR) {
        wI32 err_code = wuk::net::SystemError::code();
        throw wuk::Exception(err_code, "wuk::net::WukSocket::listen",
            wuk::net::SystemError::message(err_code).c_str());
    }
}

void wuk::net::WukSocket::send(const std::string &buffer, wI32 flag)
{
    wI32 err = ::send(this->fd, buffer.c_str(), buffer.length(), flag);
    if (err == NETERROR) {
        wI32 err_code = wuk::net::SystemError::code();
        throw wuk::Exception(err_code, "wuk::net::WukSocket::send",
            wuk::net::SystemError::message(err_code).c_str());
    }
}

std::string wuk::net::WukSocket::recv(const wU32 &length, wI32 flag)
{
    std::string buffer(length, '\0');
    wI32 err = ::recv(this->fd, buffer.data(), length, flag);
    if (err == 0) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::net::WukSocket::recv",
            "Connection closed by peer.");
    }
    if (err == NETERROR) {
        wI32 err_code = wuk::net::SystemError::code();
        throw wuk::Exception(err_code, "wuk::net::WukSocket::recv",
            wuk::net::SystemError::message(err_code).c_str());
    }
    buffer.resize(err);
    return buffer;
}

void wuk::net::WukSocket::close()
{
#   if defined(WUK_PLATFORM_WINOS)
    wI32 err = ::closesocket(this->fd);
#   else
    wI32 err = ::close(this->fd);
#   endif
    if (err == NETERROR) {
        wI32 err_code = wuk::net::SystemError::code();
        throw wuk::Exception(err_code, "wuk::net::WukSocket::close",
            wuk::net::SystemError::message(err_code).c_str());
    }
}

void wuk::net::WukSocket::set_raddr(const WukSockaddr &addr)
{
    this->m_raddr = addr;
}

void wuk::net::WukSocket::set_laddr(const WukSockaddr &addr)
{
    this->m_laddr = addr;
}
