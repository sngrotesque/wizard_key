#include <net/WukSocket.hh>
#include <WukTime.hh>
#include <cmath>

#include <iostream>

/// Basic test ///////////////////////////////////////////////////////

std::string useragent("Mozilla/5.0 (X11; Linux x86_64; rv:129.0) Gecko/20100101 Firefox/129.0");

void server_test(const std::string &addr, const wuk::u16 &port)
{
    wuk::net::Socket server(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    server.setsockopt<wuk::i32>(SOL_SOCKET, SO_REUSEADDR, true);
    server.bind(addr, port);
    server.listen(5);

    fmt::print("等待连接...\n");
    wuk::net::Socket client = server.accept();
    fmt::print("客户端已连接：{}:{}。\n",
        client.get_raddr().get_address(),
        client.get_raddr().get_port()
    );

    wuk::Buffer data = client.recv(5);
    fmt::print("接收到数据：{}\n", data.to_str());

    client.send("hello");
    client.close();

    server.close();
}

void client_test(const std::string &addr, const wuk::u16 &port)
{
    wuk::net::Socket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    fd.connect(addr, port);

    std::string headers = fmt::format(
        "GET / HTTP/1.1\r\n"
        "Host: {}:{}\r\n"
        "Accept: */*\r\n"
        "User-Agent: Android\r\n\r\n",
        addr, port
    );

    fd.send(headers);

    std::cout << fd.recv(4096).to_str() << std::endl;

    fd.close();
}

void udp_test(const std::string &addr, const wuk::u16 &port)
{
    wuk::net::Socket fd(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    wuk::net::Addrinfo ainfo(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    wuk::net::Sockaddr remote;

    fd.bind("0.0.0.0", 48999);

    std::cout << "send...\n";
    fd.sendto("hello", ainfo.resolve(addr, port).get_sockaddr());
    std::cout << "recv...\n";
    fd.recvfrom(5, remote);
    std::cout << "close...\n";
    fd.close();
}

void block_test(const std::string &addr, const wuk::u16 &port)
{
    wuk::net::Socket fd(AF_INET, SOCK_STREAM, 0);

    fd.set_blocking(false);
    fd.set_timeout(1);

    fd.connect(addr, port);

    std::string headers = fmt::format(
        "GET / HTTP/1.1\r\n"
        "Host: {}:{}\r\n"
        "Accept: */*\r\n"
        "User-Agent: Android\r\n\r\n",
        addr, port
    );

    fd.send(headers);

    std::cout << fd.recv(4096).to_str() << std::endl;

    fd.close();
}

void timeout_test(const std::string &addr, const wuk::u16 &port, wuk::f64 timeout = 3)
{
    wuk::net::Socket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    fd.set_timeout(timeout);

    
    std::string headers = fmt::format(
        "GET / HTTP/1.1\r\n"
        "Host: {}:{}\r\n"
        "Accept: */*\r\n"
        "Connection: close\r\n"
        "User-Agent: {}\r\n\r\n",
        addr, port,
        useragent
    );

    fd.connect(addr, port);
    fd.sendall(headers);
    std::cout << fd.recv(4096).to_str() << std::endl;

    fd.close();
}

template <bool test_recv_instead_of_recvfrom>
void zero_test(const std::string &addr, const wuk::u16 &port)
{
    wuk::net::Socket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    fd.connect(addr, port);

    [[maybe_unused]] wuk::net::Sockaddr remote;

    std::string headers = fmt::format(
        "GET / HTTP/1.1\r\n"
        "Host: {}:{}\r\n"
        "Accept: */*\r\n"
        "Connection: close\r\n"
        "User-Agent: {}\r\n\r\n",
        addr, port,
        useragent
    );

    fd.send(headers);
    auto buffer = (test_recv_instead_of_recvfrom) ? fd.recv(0) : fd.recvfrom(0, remote);

    fmt::print("buffer size:     {}。\n", buffer.size());
    fmt::print("buffer capacity: {}。\n", buffer.capacity());
}

/// SSL test ///////////////////////////////////////////////////////

#include <core/WukLIBSSL.hh>

void throw_ssl_exception(const std::string &func_name)
{
    wuk::u32 code = ERR_get_error();
    std::string message(256, '\0');
    ERR_error_string(code, message.data());
    throw wuk::Exception(code, func_name, message);
}

class SSLSocket {
protected:
    SSL     *ssl = nullptr;
    wuk::net::Socket fd;

public:
    SSLSocket() = default;

    SSLSocket(SSL *ssl, wuk::net::Socket &&fd)
        : ssl(ssl)
        , fd(std::move(fd))
    {
        
    }

    ~SSLSocket()
    {
        // SSL_free(this->ssl);
    }

    SSLSocket(const SSLSocket &other) = delete;
    SSLSocket(SSLSocket &&other) = default;
    SSLSocket &operator=(const SSLSocket &other) = delete;
    SSLSocket &operator=(SSLSocket &&other) = default;

    void connect(const std::string &addr, wuk::u16 port)
    {
        this->fd.connect(addr, port);
        if (SSL_connect(this->ssl) != 1) {
            throw_ssl_exception("SSLSocket::connect");
        }
    }

    wuk::ilong send(const wuk::Buffer &buffer)
    {
        wuk::ilong sent = SSL_write(this->ssl, buffer.c_str(), buffer.size());
        if (sent <= 0) {
            throw_ssl_exception("SSLSocket::send");
        }
        return sent;
    }

    wuk::Buffer recv(socklen_t length)
    {
        wuk::Buffer buffer(length);

        wuk::ilong received = SSL_read(this->ssl, buffer.write<char>(length), length);
        if (received <= 0) {
            throw_ssl_exception("SSLSocket::recv");
        }

        buffer.resize(received);

        return buffer;
    }
};

class SSLContext {
private:
    SSL_CTX *ctx = nullptr;
    SSL     *ssl = nullptr;

public:
    SSLContext(const SSL_METHOD *method = nullptr)
    {
        if (method == nullptr) {
            method = TLS_method();
        }
        this->ctx = SSL_CTX_new(method);
        if (this->ctx == nullptr) {
            throw_ssl_exception("SSLContext::SSLContext");
        }

        wuk::Buffer ssl_sid("SSLContext::SSLContext");

        // 设置选项
        wuk::u64 options = SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3;
        // Strict mode
        options |= SSL_OP_NO_TLSv1                 | SSL_OP_NO_TLSv1_1;
        options |= SSL_OP_CIPHER_SERVER_PREFERENCE | SSL_OP_NO_RENEGOTIATION;
#       ifdef SSL_OP_NO_COMPRESSION
        options |= SSL_OP_NO_COMPRESSION;
#       endif
#       ifdef SSL_OP_CIPHER_SERVER_PREFERENCE
        options |= SSL_OP_CIPHER_SERVER_PREFERENCE;
#       endif
#       ifdef SSL_OP_SINGLE_DH_USE
        options |= SSL_OP_SINGLE_DH_USE;
#       endif
#       ifdef SSL_OP_SINGLE_ECDH_USE
        options |= SSL_OP_SINGLE_ECDH_USE;
#       endif
        SSL_CTX_set_options(this->ctx, options);

        // 设置密钥级别
        SSL_CTX_set_cipher_list(this->ctx, "HIGH:!aNULL:!eNULL");
        // 设置TLS协议最低版本
        SSL_CTX_set_min_proto_version(this->ctx, TLS1_2_VERSION);
        // 设置TLS协议模式
        SSL_CTX_set_mode(this->ctx, SSL_MODE_RELEASE_BUFFERS);
        // 设置TLS协议会话上下文ID
        SSL_CTX_set_session_id_context(this->ctx, ssl_sid.data(), ssl_sid.size());

        this->ssl = SSL_new(this->ctx);
        if (this->ssl == nullptr) {
            SSL_CTX_free(this->ctx);
            throw_ssl_exception("SSLContext::SSLContext");
        }
    }

    ~SSLContext()
    {
        // SSL_CTX_free(this->ctx);
    }

    SSLContext(const SSLContext &other) = delete;
    SSLContext(SSLContext &&other) = default;
    SSLContext &operator=(const SSLContext &other) = delete;
    SSLContext &operator=(SSLContext &&other) = default;

    SSLSocket wrap_socket(wuk::net::Socket &&fd, const char *server_hostname)
    {
        if (SSL_set_fd(this->ssl, fd.get_fd()) != 1) {
            throw_ssl_exception("SSLContext::wrap_socket");
        }
        if (SSL_set_tlsext_host_name(this->ssl, server_hostname) != 1) {
            throw_ssl_exception("SSLContext::wrap_socket");
        }
        return SSLSocket(this->ssl, std::move(fd));
    }

    void destroy()
    {
        SSL_free(this->ssl);
        SSL_CTX_free(this->ctx);
    }
};

void https_test(const std::string &addr, const wuk::u16 &port)
{
    SSLContext context;
    SSLSocket ssl_fd;
    wuk::net::Socket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    ssl_fd = context.wrap_socket(std::move(fd), addr.c_str());

    ssl_fd.connect(addr, port);

    std::string headers = fmt::format(
        "GET /qrcode/GetLoginUrl HTTP/1.1\r\n"
        "Host: {}\r\n"
        "Accept: */*\r\n"
        "User-Agent: {}\r\n\r\n",
        addr, useragent
    );

    ssl_fd.send(headers);
    auto buffer = ssl_fd.recv(4096);

    std::cout << buffer.to_str() << std::endl;

    context.destroy();
}

int main()
{
#   ifdef WUK_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    std::cout << "The program starts execution.\n";

    try {
        https_test("passport.bilibili.com", 443);
    } catch (const wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
    }

#   ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
