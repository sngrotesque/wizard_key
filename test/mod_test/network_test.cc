#include <net/WukSocket.hh>
#ifdef WUK_PACKET_TEST
#   include <net/WukPacket.hh>
#endif
#include <WukTime.hh>
#include <cmath>

#include <iostream>
#include <sstream>

namespace wn = wuk::net;

void server_test(const std::string &addr, const wuk::u16 &port)
{
    wuk::net::Socket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    fd.setsockopt<bool>(SOL_SOCKET, SO_REUSEADDR, true);
    std::cout << "getsockopt: " << fd.getsockopt<bool>(SOL_SOCKET, SO_REUSEADDR) << std::endl;
    fd.bind(addr, port);
    fd.listen(5);

    std::cout << "waiting connecting...\n";
    auto client_fd = fd.accept();
    std::cout << client_fd.recv(5) << std::endl;
    client_fd.send("hello");
    client_fd.close();

    fd.close();
}

void client_test(const std::string &addr, const wuk::u16 &port)
{
    wuk::net::Socket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    fd.connect(addr, port);

    std::stringstream ss;
    ss  << "GET / HTTP/1.1\r\n"
        << "Host: " << addr << "\r\n"
        << "Accept: */*\r\n"
        << "User-Agent: Android\r\n\r\n";
    std::string headers = ss.str();

    fd.send(headers);

    std::cout << fd.recv(4096) << std::endl;

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

#ifdef WUK_PACKET_TEST
void protobuf_test(const std::string &addr, const wuk::u16 &port)
{
    wuk::net::Socket sock(AF_INET, SOCK_STREAM, 0);
    wuk::net::Packet packet;
    wuk::Time time;

    std::cout << "Build message package...\n";
    packet.set_type(wuk::net::MessageType::MESSAGE)
          .set_timestamp(time.time())
          .set_ids(123, 123)
          .set_proto_ver(0x01);
    std::cout << "Connection...\n";
    sock.connect(addr, port);
    std::cout << "Sendall...\n";
    sock.sendall(packet.serialize());
    std::cout << "Recv...\n";
    std::cout << "The remote host returns a message: "
              << sock.recv(1024)
              << std::endl;
    std::cout << "Close...\n";
    sock.close();
}
#endif

void block_test(const std::string &addr, const wuk::u16 &port)
{
    wuk::net::Socket fd(AF_INET, SOCK_STREAM, 0);

    fd.set_blocking(false);

    fd.connect(addr, port);

    std::stringstream ss;
    ss  << "GET / HTTP/1.1\r\n"
        << "Host: " << addr << "\r\n"
        << "Accept: */*\r\n"
        << "User-Agent: Android\r\n\r\n";
    std::string headers = ss.str();

    fd.send(headers);

    std::cout << fd.recv(4096) << std::endl;

    fd.close();
}

void timeout_test(const std::string &addr, const wuk::u16 &port, wuk::f64 timeout = 3)
{
    wuk::net::Socket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    fd.set_timeout(timeout);

    std::string useragent("Mozilla/5.0 (X11; Linux x86_64; rv:129.0) Gecko/20100101 Firefox/129.0");
    std::stringstream headers;
    headers << "GET / HTTP/1.1\r\n"
            << "Host: " << addr << ":" << port << "\r\n"
            << "Accept: */*\r\n"
            << "Connection: close\r\n"
            << "User-Agent: " << useragent << "\r\n"
            << "\r\n";

#   if defined(CONNECT_TEST)
    fd.connect_ex(addr, port);

    fd.send(headers.str());

    std::cout << fd.recv(4096) << std::endl;
#   elif defined(ACCEPT_TEST)
    fd.bind(addr, port);
    fd.listen(5);
    auto client = fd.accept_ex();

    std::cout << client.recv(4096) << std::endl;
    client.send(headers.str());

    client.close();
#   elif defined(SEND_TEST)
    fd.connect(addr, port);

    fd.send_ex(headers.str());

    std::cout << fd.recv(4096) << std::endl;
#   elif defined(RECV_TEST)
    fd.connect(addr, port);

    fd.send(headers.str());

    std::cout << fd.recv_ex(4096) << std::endl;
#   elif defined(ALL_TEST)
    fd.connect_ex(addr, port);
    fd.send_ex(headers.str());
    std::cout << fd.recv_ex(4096) << std::endl;
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
        timeout_test("klbq.idreamsky.com", 80, 0.001 * 1e3);
    } catch (const wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
    }

#   ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
