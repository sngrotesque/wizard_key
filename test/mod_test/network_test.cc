#include <net/WukSocket.hh>
#include <WukTime.hh>
#include <cmath>

#include <iostream>

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

    std::string useragent("Mozilla/5.0 (X11; Linux x86_64; rv:129.0) Gecko/20100101 Firefox/129.0");
    std::string headers = fmt::format(
        "GET / HTTP/1.1\r\n"
        "Host: {}:{}\r\n"
        "Accept: */*\r\n"
        "Connection: close\r\n"
        "User-Agent: {}\r\n\r\n",
        addr, port,
        useragent
    );

#   if defined(CONNECT_TEST)
    fd.connect_ex(addr, port);

    fd.send(headers);

    std::cout << fd.recv(4096).to_str() << std::endl;
#   elif defined(ACCEPT_TEST)
    fd.bind(addr, port);
    fd.listen(5);
    auto client = fd.accept_ex();

    std::cout << client.recv(4096).to_str() << std::endl;
    client.send(headers);

    client.close();
#   elif defined(SEND_TEST)
    fd.connect(addr, port);

    fd.sendall_ex(headers);

    std::cout << fd.recv(4096).to_str() << std::endl;
#   elif defined(RECV_TEST)
    fd.connect(addr, port);

    fd.send(headers);

    std::cout << fd.recv_ex(4096).to_str() << std::endl;
#   elif defined(ALL_TEST)
    fd.connect_ex(addr, port);
    fd.sendall_ex(headers);
    std::cout << fd.recv_ex(4096).to_str() << std::endl;
#   endif

    fd.close();
}

int main()
{
#   ifdef WUK_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    std::cout << "The program starts execution.\n";

    try {
        timeout_test("klbq.idreamsky.com", 80, 1 / 1e6);
    } catch (const wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
    }

#   ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
