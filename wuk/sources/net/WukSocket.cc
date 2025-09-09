#include <net/WukSocket.hh>

#include <WukMemory.hh>
#include <fcntl.h>

namespace wuk::net {
// ==================== Addrinfo ====================
    Addrinfo::Addrinfo(wuk::i32 family, wuk::i32 sock_type, wuk::i32 proto)
    {
        this->hints.ai_family = family;
        this->hints.ai_socktype = sock_type;
        this->hints.ai_protocol = proto;
    }

    Addrinfo::~Addrinfo()
    {
        freeaddrinfo(this->res);
    }

    Addrinfo &Addrinfo::resolve(const std::string &addr, const wuk::u16 &port)
    {
        freeaddrinfo(this->res); // 防止多次调用导致内存泄漏

        wuk::i32 err_code = getaddrinfo(addr.c_str(), std::to_string(port).c_str(),
                &this->hints, &this->res);
        if (err_code) {
            throw wuk::Exception(err_code, "wuk::net::Addrinfo::resolve",
                err::system::message(err_code));
        }

        return *this; // 返回自身方便链式调用
    }

    const sockaddr *Addrinfo::get_addr() const
    {
        if (!this->res) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::net::Addrinfo::get_addr",
                "this->res is nullptr.");
        }
        return this->res->ai_addr;
    }

    socklen_t Addrinfo::get_addrlen() const
    {
        if (!this->res) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::net::Addrinfo::get_addrlen",
                "this->res is nullptr.");
        }
        return this->res->ai_addrlen;
    }

    Sockaddr Addrinfo::get_sockaddr() const
    {
        if (!this->res) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::net::Addrinfo::get_sockaddr",
                "this->res is nullptr.");
        }
        return Sockaddr(this->res->ai_addr, this->res->ai_addrlen);
    }

// ==================== Sockaddr ====================
    Sockaddr::Sockaddr(const sockaddr_storage *addr, const socklen_t &addrlen)
    {
        this->set_addr(reinterpret_cast<const sockaddr *>(addr), addrlen);
    }

    Sockaddr::Sockaddr(const sockaddr *addr, const socklen_t &addrlen)
    {
        this->set_addr(addr, addrlen);
    }

    Sockaddr::~Sockaddr()
    {
        wuk::memory_zero(&this->addr, this->addrlen);
    }

    sockaddr *Sockaddr::set_addr() noexcept
    {
        return reinterpret_cast<sockaddr *>(&this->addr);
    }

    socklen_t *Sockaddr::set_addrlen() noexcept
    {
        return &this->addrlen;
    }

    void Sockaddr::set_addr(const sockaddr *addr, const socklen_t &addrlen)
    {
        if (!addr || addrlen > static_cast<socklen_t>(sizeof(sockaddr_storage))) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::net::Sockaddr::set_addr",
                "Invalid address or length");
        }
        memcpy(&this->addr, addr, addrlen);
        this->addrlen = addrlen;
    }

    void Sockaddr::set_addr(const Sockaddr &addr)
    {
        this->set_addr(addr.get_addr(), addr.get_addrlen());
    }

    const sockaddr *Sockaddr::get_addr() const noexcept
    {
        return reinterpret_cast<const sockaddr *>(&this->addr);
    }

    socklen_t Sockaddr::get_addrlen() const noexcept
    {
        return this->addrlen;
    }

    const std::string Sockaddr::get_address_string() const
    {
        char buffer[INET6_ADDRSTRLEN] = {0};
        const sockaddr *sa = this->get_addr();
        
        if (!sa) {
            return std::string{};
        }

        auto throw_error = []() -> void {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Sockaddr::get_address_string",
                err::system::message(err_code));
        };

        switch (sa->sa_family) {
            case AF_INET:
            {
                const sockaddr_in *sin = reinterpret_cast<const sockaddr_in *>(sa);
                if (!inet_ntop(AF_INET, &sin->sin_addr, buffer, sizeof(buffer))) {
                    throw_error();
                }
                break;
            }
            case AF_INET6:
            {
                const sockaddr_in6 *sin6 = reinterpret_cast<const sockaddr_in6 *>(sa);
                if (!inet_ntop(AF_INET6, &sin6->sin6_addr, buffer, sizeof(buffer))) {
                    throw_error();
                }
                break;
            }
            default:
                throw wuk::Exception(wuk::Error::ERR,
                    "wuk::net::Sockaddr::get_address_string",
                    "Unsupported address family");
        }

        return std::string(buffer);
    }

    wuk::u16 Sockaddr::get_port() const
    {
        const sockaddr *sa = this->get_addr();

        if (!sa) {
            return 0;
        }

        switch (sa->sa_family) {
            case AF_INET:
            {
                const sockaddr_in *sin = reinterpret_cast<const sockaddr_in *>(sa);
                return ntohs(sin->sin_port);
            }
            case AF_INET6:
            {
                const sockaddr_in6 *sin6 = reinterpret_cast<const sockaddr_in6 *>(sa);
                return ntohs(sin6->sin6_port);
            }
            default:
                throw wuk::Exception(wuk::Error::ERR,
                    "wuk::net::Sockaddr::get_port",
                    "Unsupported address family");
        }
    }

// ==================== Socket ====================
    Socket::Socket(wuk::i32 family, wuk::i32 sock_type, wuk::i32 proto)
    : m_family(family), m_sock_type(sock_type), m_proto(proto)
    {
        this->fd = socket(family, sock_type, proto);
        if (this->fd == static_cast<wSocket>(NETERROR)) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::Socket",
                err::system::message(err_code));
        }
    }

    Socket::Socket(wuk::i32 family, wuk::i32 sock_type, wuk::i32 proto, wSocket cur_fd)
    : m_family(family), m_sock_type(sock_type), m_proto(proto)
    {
        this->fd = cur_fd;
        this->m_laddr = this->getsockname();
    }

    Socket::~Socket()
    {
        try {
            this->close();
        } catch (...) {
            // 析构函数不应该抛出异常
        }
    }

    Socket::Socket(Socket &&other) noexcept
    {
        this->fd = other.fd;
        this->m_family = other.m_family;
        this->m_sock_type = other.m_sock_type;
        this->m_proto = other.m_proto;
        this->m_raddr = other.m_raddr;
        this->m_laddr = other.m_laddr;
        this->m_timeout = other.m_timeout;
        this->is_close = other.is_close;
        this->is_blocking = other.is_blocking;

        other.mark_invalid();
    }

    Socket &Socket::operator=(Socket &&other) noexcept
    {
        if (this != &other) {
            this->close();
            
            this->fd = other.fd;
            this->m_family = other.m_family;
            this->m_sock_type = other.m_sock_type;
            this->m_proto = other.m_proto;
            this->m_raddr = other.m_raddr;
            this->m_laddr = other.m_laddr;
            this->m_timeout = other.m_timeout;
            this->is_close = other.is_close;
            this->is_blocking = other.is_blocking;

            other.mark_invalid();
        }
        return *this;
    }

#   ifdef WUK_STD_CPP_20
    std::strong_ordering Socket::operator<=>(const Socket &other) const
    {
        return this->fd <=> other.fd;
    }
#   else
    bool Socket::operator<(const Socket &other) const
    {
        return this->fd < other.fd;
    }

    bool Socket::operator<=(const Socket &other) const
    {
        return this->fd <= other.fd;
    }

    bool Socket::operator>(const Socket &other) const
    {
        return this->fd > other.fd;
    }

    bool Socket::operator>=(const Socket &other) const
    {
        return this->fd >= other.fd;
    }

    bool Socket::operator==(const Socket &other) const
    {
        return this->fd == other.fd;
    }

    bool Socket::operator!=(const Socket &other) const
    {
        return this->fd != other.fd;
    }
#   endif

    const Sockaddr Socket::getsockname()
    {
        Sockaddr addr;
        wuk::i32 err = ::getsockname(this->fd, addr.set_addr(), addr.set_addrlen());
        if (err == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::getsockname",
                err::system::message(err_code));
        }
        return addr;
    }

    void Socket::set_blocking(bool blocked)
    {
        wuk::i32 err = 0;

#       ifdef WUK_PLATFORM_WINOS
        wuk::u32 mode = static_cast<wuk::u32>(!blocked);
        err = ioctlsocket(this->fd, FIONBIO, reinterpret_cast<u_long *>(&mode));
#       else
        wuk::i32 flag = fcntl(this->fd, F_GETFL, 0);
        if (flag == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::set_blocking",
                err::system::message(err_code));
        }
        if (blocked) {
            flag &= ~O_NONBLOCK;
        } else {
            flag |= O_NONBLOCK;
        }
        err = fcntl(this->fd, F_SETFL, flag);
#       endif
        if (err == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::set_blocking",
                err::system::message(err_code));
        }

        this->is_blocking = blocked;
    }

    bool Socket::get_blocking() const noexcept
    {
        return this->is_blocking;
    }

    void Socket::set_timeout(wuk::f64 t) noexcept
    {
        this->m_timeout = (t < 0) ? 0 : t; // 超时时间不允许为负数
    }

    wuk::f64 Socket::get_timeout() const noexcept
    {
        return this->m_timeout;
    }

    void Socket::connect(const std::string &addr, const wuk::u16 &port)
    {
        Addrinfo info(this->m_family);
        info.resolve(addr, port);
        wuk::i32 err = ::connect(this->fd, info.get_addr(), info.get_addrlen());
        if (err == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::connect",
                err::system::message(err_code));
        }
        this->m_raddr.set_addr(info.get_addr(), info.get_addrlen());
        this->m_laddr.set_addr(this->getsockname());
    }

    void Socket::bind(const std::string &addr, const wuk::u16 &port)
    {
        Addrinfo info(this->m_family);
        info.resolve(addr, port);
        wuk::i32 err = ::bind(this->fd, info.get_addr(), info.get_addrlen());
        if (err == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::bind",
                err::system::message(err_code));
        }
        this->m_laddr.set_addr(info.get_addr(), info.get_addrlen());
    }

    Socket Socket::accept() const
    {
        Sockaddr client;

        wSocket client_sock = ::accept(this->fd, client.set_addr(), client.set_addrlen());

        if (client_sock == static_cast<wSocket>(NETERROR)) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::accept",
                err::system::message(err_code));
        }

        Socket new_sock(this->m_family, this->m_sock_type, this->m_proto, client_sock);
        new_sock.set_raddr(client);

        return new_sock;
    }

    void Socket::listen(const socklen_t &backlog)
    {
        wuk::i32 err = ::listen(this->fd, backlog);
        if (err == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::listen",
                err::system::message(err_code));
        }
    }

    wuk::ilong Socket::send(const std::string &buffer, wuk::i32 flag)
    {
        wuk::ilong sent = ::send(this->fd, buffer.c_str(), buffer.length(), flag);
        if (sent == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::send",
                err::system::message(err_code));
        }
        return sent;
    }

    std::string Socket::recv(const socklen_t &length, wuk::i32 flag)
    {
        std::string buffer(length, '\0');
        wuk::ilong received = ::recv(this->fd, buffer.data(), length, flag);
        if (received == 0) {
            return std::string{};
        }
        if (received == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::recv",
                err::system::message(err_code));
        }
        buffer.resize(received);
        return buffer;
    }

    void Socket::sendall(const std::string &buffer, wuk::i32 flag)
    {
        constexpr wuk::ilong block_size = 2048;
        const char *data_ptr = buffer.c_str();
        wuk::ilong data_len = static_cast<wuk::ilong>(buffer.length());
        while (data_len) {
            wuk::ilong size = wuk::min(block_size, data_len);
            wuk::ilong sent = ::send(this->fd, data_ptr, size, flag);
            if (sent == NETERROR) {
                wuk::i32 err_code = err::system::code();
                throw wuk::Exception(err_code, "wuk::net::Socket::sendall",
                    err::system::message(err_code));
            }
            data_ptr += sent;
            data_len -= sent;
        }
    }

    wuk::ilong Socket::sendto(const std::string &buffer, const Sockaddr &addr, wuk::i32 flag)
    {
        wuk::ilong sent = ::sendto(this->fd, buffer.c_str(), buffer.length(), flag,
                            addr.get_addr(), addr.get_addrlen());
        if (sent == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::sendto",
                err::system::message(err_code));
        }
        return sent;
    }

    std::string Socket::recvfrom(const socklen_t &length, Sockaddr &addr, wuk::i32 flag)
    {
        std::string buffer(length, '\0');
        wuk::ilong received = ::recvfrom(this->fd, buffer.data(), length, flag,
                                    addr.set_addr(), addr.set_addrlen());
        if (received == 0) {
            return std::string{};
        }
        if (received == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::recvfrom",
                err::system::message(err_code));
        }
        buffer.resize(received);
        return buffer;
    }

    void Socket::shutdown(const wuk::i32 &how)
    {
        if (::shutdown(this->fd, how) == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::shutdown",
                err::system::message(err_code));
        }
    }

    void Socket::close()
    {
        if (!this->is_valid()) {
            return;
        }

#       if defined(WUK_PLATFORM_WINOS)
        wuk::i32 err = ::closesocket(this->fd);
#       else
        wuk::i32 err = ::close(this->fd);
#       endif
        if (err == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::close",
                err::system::message(err_code));
        }

        this->is_close = true;
        this->fd = static_cast<wSocket>(NETERROR);
    }

    void Socket::set_raddr(const Sockaddr &addr)
    {
        this->m_raddr = addr;
    }

    void Socket::set_laddr(const Sockaddr &addr)
    {
        this->m_laddr = addr;
    }

    const Sockaddr &Socket::get_raddr() const noexcept
    {
        return this->m_raddr;
    }

    const Sockaddr &Socket::get_laddr() const noexcept
    {
        return this->m_laddr;
    }

    wSocket Socket::get_fd() const noexcept
    {
        return this->fd;
    }

    bool Socket::is_valid() const noexcept
    {
        if (this->is_close || this->fd == static_cast<wSocket>(NETERROR)) {
            return false;
        }

        wuk::i32 error = 0;
        socklen_t error_size = sizeof(error);
        if (::getsockopt(this->fd, SOL_SOCKET, SO_ERROR,
                            reinterpret_cast<char *>(&error), &error_size) != 0) {
            return false;
        }

        return error == 0;
    }

    void Socket::mark_invalid() noexcept
    {
        this->fd = static_cast<wSocket>(NETERROR);

        this->m_family = NETERROR;
        this->m_sock_type = NETERROR;
        this->m_proto = NETERROR;

        this->m_timeout = 0;

        this->is_close = true; // 此处不应调用close
    }
}
