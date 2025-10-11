#include <net/WukSocket.hh>

#include <WukMemory.hh>
#include <fcntl.h>

namespace wuk::net {
// ==================== Addrinfo ====================
    Addrinfo::Addrinfo(wuk::i32 family, wuk::i32 sock_type, wuk::i32 proto) noexcept
    {
        this->m_hints.ai_family = family;
        this->m_hints.ai_socktype = sock_type;
        this->m_hints.ai_protocol = proto;
    }

    Addrinfo::~Addrinfo()
    {
        freeaddrinfo(this->m_res);
    }

    Addrinfo::Addrinfo(Addrinfo &&other) noexcept
        : m_hints(other.m_hints)
        , m_res(other.m_res)
    {
        other.m_res = nullptr;
    }

    Addrinfo &Addrinfo::operator=(Addrinfo &&other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        this->m_hints = other.m_hints;
        this->m_res = other.m_res;
        other.m_res = nullptr;
        return *this;
    }

    Addrinfo &Addrinfo::resolve(const std::string &addr, const wuk::u16 &port)
    {
        freeaddrinfo(this->m_res); // 防止多次调用导致内存泄漏

        wuk::i32 err_code = getaddrinfo(addr.c_str(), std::to_string(port).c_str(),
                &this->m_hints, &this->m_res);
        if (err_code) {
            throw wuk::Exception(err_code, "wuk::net::Addrinfo::resolve",
                err::system::message(err_code));
        }

        return *this; // 返回自身方便链式调用
    }

    const sockaddr *Addrinfo::get_addr() const
    {
        if (!this->m_res) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::net::Addrinfo::get_addr",
                "this->m_res is nullptr.");
        }
        return this->m_res->ai_addr;
    }

    socklen_t Addrinfo::get_addrlen() const
    {
        if (!this->m_res) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::net::Addrinfo::get_addrlen",
                "this->m_res is nullptr.");
        }
        return this->m_res->ai_addrlen;
    }

    Sockaddr Addrinfo::get_sockaddr() const
    {
        if (!this->m_res) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::net::Addrinfo::get_sockaddr",
                "this->m_res is nullptr.");
        }
        return Sockaddr(this->m_res->ai_addr, this->m_res->ai_addrlen);
    }

// ==================== Sockaddr ====================
    Sockaddr::Sockaddr(const sockaddr_storage *addr, socklen_t addrlen)
    {
        this->set_addr(reinterpret_cast<const sockaddr *>(addr), addrlen);
    }

    Sockaddr::Sockaddr(const sockaddr *addr, socklen_t addrlen)
    {
        this->set_addr(addr, addrlen);
    }

    Sockaddr::~Sockaddr()
    {
        wuk::memory_zero(&this->m_addr, this->m_addrlen);
    }

    sockaddr *Sockaddr::set_addr() noexcept
    {
        return reinterpret_cast<sockaddr *>(&this->m_addr);
    }

    socklen_t *Sockaddr::set_addrlen() noexcept
    {
        return &this->m_addrlen;
    }

    void Sockaddr::set_addr(const sockaddr *addr, socklen_t addrlen)
    {
        if (!addr || addrlen > static_cast<socklen_t>(sizeof(sockaddr_storage))) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::net::Sockaddr::set_addr",
                "Invalid address or length");
        }
        memcpy(&this->m_addr, addr, addrlen);
        this->m_addrlen = addrlen;
    }

    void Sockaddr::set_addr(const Sockaddr &addr)
    {
        this->set_addr(addr.get_addr(), addr.get_addrlen());
    }

    const sockaddr *Sockaddr::get_addr() const noexcept
    {
        return reinterpret_cast<const sockaddr *>(&this->m_addr);
    }

    socklen_t Sockaddr::get_addrlen() const noexcept
    {
        return this->m_addrlen;
    }

    const std::string Sockaddr::get_address() const
    {
        char buffer[INET6_ADDRSTRLEN] = {0};
        const sockaddr *sa = this->get_addr();
        
        if (!sa) {
            return std::string{};
        }

        auto throw_error = []() -> void {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Sockaddr::get_address",
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
                    "wuk::net::Sockaddr::get_address",
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
        : m_family(family)
        , m_sock_type(sock_type)
        , m_proto(proto)
    {
        this->m_fd = socket(family, sock_type, proto);
        if (this->m_fd == INV_SOCK) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::Socket",
                err::system::message(err_code));
        }
    }

    Socket::Socket(wuk::i32 family, wuk::i32 sock_type, wuk::i32 proto, wSocket other_fd)
        : m_fd(other_fd)
        , m_family(family)
        , m_sock_type(sock_type)
        , m_proto(proto)
    {
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
        this->m_fd = other.m_fd;
        this->m_family = other.m_family;
        this->m_sock_type = other.m_sock_type;
        this->m_proto = other.m_proto;
        this->m_raddr = other.m_raddr;
        this->m_laddr = other.m_laddr;
        this->m_timeout = other.m_timeout;
        this->m_is_close = other.m_is_close;
        this->m_is_blocking = other.m_is_blocking;

        other.mark_invalid();
    }

    Socket &Socket::operator=(Socket &&other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        this->close();

        this->m_fd = other.m_fd;
        this->m_family = other.m_family;
        this->m_sock_type = other.m_sock_type;
        this->m_proto = other.m_proto;
        this->m_raddr = other.m_raddr;
        this->m_laddr = other.m_laddr;
        this->m_timeout = other.m_timeout;
        this->m_is_close = other.m_is_close;
        this->m_is_blocking = other.m_is_blocking;

        other.mark_invalid();
        return *this;
    }

#   ifdef WUK_STD_CPP_20
    std::strong_ordering Socket::operator<=>(const Socket &other) const noexcept
    {
        return this->m_fd <=> other.m_fd;
    }
#   else
    bool Socket::operator<(const Socket &other) const noexcept
    {
        return this->m_fd < other.m_fd;
    }

    bool Socket::operator<=(const Socket &other) const noexcept
    {
        return this->m_fd <= other.m_fd;
    }

    bool Socket::operator>(const Socket &other) const noexcept
    {
        return this->m_fd > other.m_fd;
    }

    bool Socket::operator>=(const Socket &other) const noexcept
    {
        return this->m_fd >= other.m_fd;
    }

    bool Socket::operator==(const Socket &other) const noexcept
    {
        return this->m_fd == other.m_fd;
    }

    bool Socket::operator!=(const Socket &other) const noexcept
    {
        return this->m_fd != other.m_fd;
    }
#   endif

    Socket::operator bool() const noexcept
    {
        return this->is_valid();
    }

    const Sockaddr Socket::getsockname()
    {
        Sockaddr addr;
        wuk::i32 err = ::getsockname(this->m_fd, addr.set_addr(), addr.set_addrlen());
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
        err = ioctlsocket(this->m_fd, FIONBIO, reinterpret_cast<u_long *>(&mode));
#       else
        wuk::i32 flag = fcntl(this->m_fd, F_GETFL, 0);
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
        err = fcntl(this->m_fd, F_SETFL, flag);
#       endif
        if (err == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::set_blocking",
                err::system::message(err_code));
        }

        this->m_is_blocking = blocked;
    }

    bool Socket::get_blocking() const noexcept
    {
        return this->m_is_blocking;
    }

    void Socket::set_timeout(wuk::f64 t)
    {
        if (t < 0) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::net::Socket::set_timeout",
                "Timeout should not be set to a negative number.");
        }
        this->m_timeout = t;
    }

    wuk::f64 Socket::get_timeout() const noexcept
    {
        return this->m_timeout;
    }

    void Socket::connect(const std::string &addr, wuk::u16 port)
    {
        Addrinfo info(this->m_family);
        info.resolve(addr, port);
        wuk::i32 err = ::connect(this->m_fd, info.get_addr(), info.get_addrlen());
        if (err == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::connect",
                err::system::message(err_code));
        }
        this->m_raddr.set_addr(info.get_addr(), info.get_addrlen());
        this->m_laddr.set_addr(this->getsockname());
    }

    void Socket::bind(const std::string &addr, wuk::u16 port)
    {
        Addrinfo info(this->m_family);
        info.resolve(addr, port);
        wuk::i32 err = ::bind(this->m_fd, info.get_addr(), info.get_addrlen());
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
        wSocket client_sock = ::accept(this->m_fd, client.set_addr(), client.set_addrlen());

        if (client_sock == INV_SOCK) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::accept",
                err::system::message(err_code));
        }

        Socket new_sock(this->m_family, this->m_sock_type, this->m_proto, client_sock);
        new_sock.set_timeout(this->m_timeout);
        new_sock.set_raddr(client);

        return new_sock;
    }

    void Socket::listen(socklen_t backlog) const
    {
        wuk::i32 err = ::listen(this->m_fd, backlog);
        if (err == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::listen",
                err::system::message(err_code));
        }
    }

    wuk::ilong Socket::send(const wuk::Buffer &buffer, wuk::i32 flag) const
    {
        wuk::ilong sent = ::send(this->m_fd, buffer.c_str(), buffer.size(), flag);
        if (sent == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::send",
                err::system::message(err_code));
        }
        return sent;
    }

    void Socket::sendall(const wuk::Buffer &buffer, wuk::i32 flag) const
    {
        constexpr wuk::ilong block_size = 2048;
        const char *data_ptr = buffer.c_str();
        wuk::ilong data_len = static_cast<wuk::ilong>(buffer.size());

        while (data_len) {
            wuk::ilong size = wuk::min(block_size, data_len);
            wuk::ilong sent = ::send(this->m_fd, data_ptr, size, flag);
            if (sent == NETERROR) {
                wuk::i32 err_code = err::system::code();
                throw wuk::Exception(err_code, "wuk::net::Socket::sendall",
                    err::system::message(err_code));
            }
            data_ptr += sent;
            data_len -= sent;
        }
    }

    wuk::ilong Socket::sendto(const wuk::Buffer &buffer, const Sockaddr &addr, wuk::i32 flag) const
    {
        wuk::ilong sent = ::sendto(this->m_fd, buffer.c_str(), buffer.size(), flag,
                            addr.get_addr(), addr.get_addrlen());
        if (sent == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::sendto",
                err::system::message(err_code));
        }
        return sent;
    }

    wuk::Buffer Socket::recv(socklen_t length, wuk::i32 flag) const
    {
        wuk::Buffer buffer(length);
        wuk::ilong received = ::recv(this->m_fd, buffer.write<char>(length), length, flag);
        if (received == 0) {
            return {};
        }
        if (received == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::recv",
                err::system::message(err_code));
        }
        return buffer;
    }

    wuk::Buffer Socket::recvfrom(socklen_t length, Sockaddr &addr, wuk::i32 flag) const
    {
        wuk::Buffer buffer(length);
        wuk::ilong received = ::recvfrom(this->m_fd, buffer.write<char>(length), length, flag,
                                        addr.set_addr(), addr.set_addrlen());
        if (received == 0) {
            return {};
        }
        if (received == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::recvfrom",
                err::system::message(err_code));
        }
        return buffer;
    }

    void Socket::shutdown(wuk::i32 how) const
    {
        if (::shutdown(this->m_fd, how) == NETERROR) {
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
        wuk::i32 err = ::closesocket(this->m_fd);
#       else
        wuk::i32 err = ::close(this->m_fd);
#       endif
        if (err == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::close",
                err::system::message(err_code));
        }

        this->mark_invalid();
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
        return this->m_fd;
    }

    bool Socket::is_valid() const noexcept
    {
        if (this->m_is_close || (this->m_fd == INV_SOCK)) {
            return false;
        }

        wuk::i32 error = 0;
        socklen_t error_size = sizeof(error);
        wuk::i32 ret_val = ::getsockopt(
            this->m_fd,
            SOL_SOCKET,
            SO_ERROR,
            reinterpret_cast<char *>(&error),
            &error_size
        );

        if (ret_val != 0) {
            return false;
        }

        return error == 0;
    }

    void Socket::mark_invalid() noexcept
    {
        this->m_fd = INV_SOCK;

        this->m_family = NETERROR;
        this->m_sock_type = NETERROR;
        this->m_proto = NETERROR;

        this->m_timeout = 0;

        this->m_is_close = true; // 此处不应调用close
    }
}
