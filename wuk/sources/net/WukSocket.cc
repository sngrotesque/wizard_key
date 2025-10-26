#include <net/WukSocket.hh>

#include <WukMemory.hh>
#include <fcntl.h>

// ==================== Private ====================
enum class IOType {
    // write
    CONNECT,
    SEND,
    // read
    ACCEPT,
    RECV,
};

// 偷个懒
static inline void throw_error(const std::string &func_name)
{
    wuk::i32 err_code = wuk::net::err::system::code();
    throw wuk::Exception(err_code, func_name,
        wuk::net::err::system::message(err_code));
}

static inline timeval create_timeval(wuk::f64 timeout) noexcept
{
    timeval tv {};

    wuk::f64 int_part{}, float_part{};
    float_part = modf(timeout, &int_part);

    tv.tv_sec  = static_cast<time_t>(int_part);
    tv.tv_usec = static_cast<time_t>(float_part * 1e6);

    return tv;
}

template <typename T, typename call_function, typename ...Args>
static T sock_call_ex(
    wuk::net::Socket &fd,
    const std::string &func_name,
    call_function func,
    IOType io_type,
    Args &&... args
)
{
    using namespace wuk::net;

    if (fd.get_timeout() == 0) {
        // 如果未设置超时就按普通阻塞套接字进行操作
        return func(std::forward<Args>(args)...);
    }

    // 保存套接字的原始阻塞状态
    bool was_blocking = fd.get_blocking();

    try {
        // 将套接字设置为非阻塞模式
        fd.set_blocking(false);

        // 执行非阻塞操作
        T result = func(std::forward<Args>(args)...);
        T _res_err = NETERROR;

        if constexpr (std::is_same_v<T, wSocket>) {
            _res_err = static_cast<wSocket>(NETERROR);
        }

        if (result == _res_err) {
            err::SocketType sock_err = err::from_code(err::system::code());
            bool process =  (sock_err == err::SocketType::WOULDBLOCK) ||
                            (sock_err == err::SocketType::INPROGRESS);
            if (process) {
                // 初始化套接字集
                fd_set fds;
                FD_ZERO(&fds);
                // 将当前套接字加入到监听集合
                FD_SET(fd.get_fd(), &fds);

                timeval tv = create_timeval(fd.get_timeout());
                wuk::i32 ready;
                switch (io_type) {
                    case IOType::CONNECT:
                    case IOType::SEND:
                        ready = select(fd.get_fd() + 1, nullptr, &fds, nullptr, &tv); break;
                    case IOType::ACCEPT:
                    case IOType::RECV:
                        ready = select(fd.get_fd() + 1, &fds, nullptr, nullptr, &tv); break;
                }

                if (ready == 0) {
                    // 如果没有已就绪的套接字说明超时
                    throw wuk::Exception(wuk::Error::TIMEO, func_name,
                        "The socket has timed out.");
                } else if (ready == NETERROR) {
                    // 出错
                    throw_error(func_name);
                }

                // 处理已就绪的套接字
                if (io_type == IOType::CONNECT) {
                    // 连接操作直接获取套接字状态查看是否存在错误并恢复套接字阻塞状态
                    fd.set_blocking(was_blocking);
                    return fd.getsockopt<wuk::i32>(SOL_SOCKET, SO_ERROR);
                }
                // 其他操作（accept，send，recv等）
                result = func(std::forward<Args>(args)...);
                if (result == _res_err) {
                    // 函数返回了一个错误代码
                    throw_error(func_name);
                }
            }
        }

        // 注意恢复原始阻塞状态
        fd.set_blocking(was_blocking);
        // 返回函数返回值以便后续操作
        return result;
    } catch (...) {
        // 确保无论如何都要恢复原本的阻塞状态并重新抛出异常
        fd.set_blocking(was_blocking);
        throw;
    }
}

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

    std::string Sockaddr::get_address() const
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
        auto connect_timeout = [&](const sockaddr *addr, socklen_t addrlen)
        {
            return ::connect(this->m_fd, addr, addrlen);
        };
        Addrinfo info(this->m_family, this->m_sock_type, this->m_proto);
        info.resolve(addr, port);

        wuk::i32 err = sock_call_ex<wuk::i32>(
            *this,
            "wuk::net::Socket::connect",
            connect_timeout,
            IOType::CONNECT,
            info.get_addr(),
            info.get_addrlen()
        );

        if (err == NETERROR) {
            throw_error("wuk::net::Socket::connect");
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

    void Socket::listen(socklen_t backlog) const
    {
        wuk::i32 err = ::listen(this->m_fd, backlog);
        if (err == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::listen",
                err::system::message(err_code));
        }
    }

    Socket Socket::accept()
    {
        auto accept_timeout = [&](sockaddr *addr, socklen_t *addrlen)
        {
            return ::accept(this->m_fd, addr, addrlen);
        };
        Sockaddr client_addrinfo;

        wSocket client_sock = sock_call_ex<wSocket>(
            *this,
            "wuk::net::Socket::accept",
            accept_timeout,
            IOType::ACCEPT,
            client_addrinfo.set_addr(),
            client_addrinfo.set_addrlen()
        );

        if (client_sock == INV_SOCK) {
            throw_error("wuk::net::Socket::accept");
        }

        Socket client(this->m_family, this->m_sock_type, this->m_proto, client_sock);
        client.set_timeout(this->m_timeout);
        client.set_raddr(client_addrinfo);

        return client;
    }

    wuk::ilong Socket::send(const wuk::Buffer &buffer, wuk::i32 flag)
    {
        auto send_timeout = [&](const wuk::Buffer &buffer)
        {
            return ::send(this->m_fd, buffer.c_str(), buffer.length(), flag);
        };

        wuk::ilong sent = sock_call_ex<wuk::ilong>(
            *this,
            "wuk::net::Socket::send",
            send_timeout,
            IOType::SEND,
            buffer
        );

        if (sent == NETERROR) {
            throw_error("wuk::net::Socket::send");
        }

        return sent;
    }

    void Socket::sendall(const wuk::Buffer &buffer, wuk::i32 flag)
    {
        auto send_timeout = [&](const char *buffer, socklen_t length)
        {
            return ::send(this->m_fd, buffer, length, flag);
        };
        const char *data = buffer.c_str();
        wuk::ilong remaining = static_cast<wuk::ilong>(buffer.size());

        while (remaining > 0) {
            wuk::ilong sent = sock_call_ex<wuk::ilong>(
                *this,
                "wuk::net::Socket::sendall",
                send_timeout,
                IOType::SEND,
                data,
                remaining
            );

            data += sent;
            remaining -= sent;
        }
    }

    wuk::ilong Socket::sendto(const wuk::Buffer &buffer, const Sockaddr &addr, wuk::i32 flag)
    {
        auto sendto_timeout = [&](const wuk::Buffer &buffer, const Sockaddr &addr)
        {
            return ::sendto(this->m_fd, buffer.c_str(), buffer.size(), flag,
                            addr.get_addr(), addr.get_addrlen());
        };

        wuk::ilong sent = sock_call_ex<wuk::ilong>(
            *this,
            "wuk::net::Socket::sendto",
            sendto_timeout,
            IOType::SEND,
            buffer,
            addr
        );

        if (sent == NETERROR) {
            throw_error("wuk::net::Socket::sendto");
        }

        return sent;
    }

    wuk::Buffer Socket::recv(socklen_t length, wuk::i32 flag)
    {
        if (length < 0) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::net::Socket::recv",
                "The size of the buffer should not be negative.");
        }

        auto recv_timeout = [&](wuk::Buffer &buffer)
        {
            return ::recv(this->m_fd, buffer.write<char>(length), length, flag);
        };

        wuk::Buffer buffer(length);
        wuk::ilong received = sock_call_ex<wuk::ilong>(
            *this,
            "wuk::net::Socket::recv",
            recv_timeout,
            IOType::RECV,
            buffer
        );

        if (received == 0) {
            return {};
        }

        if (received == NETERROR) {
            throw_error("wuk::net::Socket::recv");
        }

        buffer.resize(received);

        return buffer;
    }

    wuk::Buffer Socket::recvall(socklen_t length, wuk::i32 flag)
    {
        if (length < 0) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::net::Socket::recvall",
                "The size of the buffer should not be negative.");
        }

        WUK_UNIMPL_EXCEPTION("wuk::net::Socket::recvall");
        /*
        auto recvall_timeout = [&](char *buffer, socklen_t length)
        {
            return ::recv(this->m_fd, buffer, length, flag);
        };

        wuk::Buffer result(length);
        while (true) {

        }
        */
    }

    wuk::Buffer Socket::recvfrom(socklen_t length, Sockaddr &addr, wuk::i32 flag)
    {
        if (length < 0) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::net::Socket::recvfrom",
                "The size of the buffer should not be negative.");
        }

        auto recvfrom_timeout = [&](wuk::Buffer &buffer, Sockaddr &addr)
        {
            return ::recvfrom(this->m_fd, buffer.write<char>(length), length, flag,
                            addr.set_addr(), addr.set_addrlen());
        };

        wuk::Buffer buffer(length);
        wuk::ilong received = sock_call_ex<wuk::ilong>(
            *this,
            "wuk::net::Socket::recvfrom",
            recvfrom_timeout,
            IOType::RECV,
            buffer,
            addr
        );

        if (received == 0) {
            return {};
        }

        if (received == NETERROR) {
            throw_error("wuk::net::Socket::recvfrom");
        }

        buffer.resize(received);

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

