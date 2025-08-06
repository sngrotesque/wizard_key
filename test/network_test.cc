#include <net/WukSocket.hh>
#include <net/WukPacket.hh>
#include <WukTime.hh>

#include <iostream>
#include <sstream>

namespace wn = wuk::net;

void server_test(const std::string &addr, const wU16 &port)
{
    wuk::net::WukSocket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    fd.setsockopt<bool>(SOL_SOCKET, SO_REUSEADDR, true);
    std::cout << "getsockopt: " << fd.getsockopt<bool>(SOL_SOCKET, SO_REUSEADDR) << std::endl;
    fd.bind(addr, port);
    fd.listen(5);

    std::cout << "waiting connecting...\n";
    if (auto client_fd = fd.accept()) {
        std::cout << client_fd->recv(5) << std::endl;
        client_fd->send("hello");
        client_fd->close();
    }

    fd.close();
}

void client_test(const std::string &addr, const wU16 &port)
{
    wuk::net::WukSocket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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

void udp_test(const std::string &addr, const wU16 &port)
{
    wuk::net::WukSocket fd(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    wuk::net::WukAddrinfo ainfo(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    wuk::net::WukSockaddr remote;

    fd.bind("0.0.0.0", 48999);

    std::cout << "send...\n";
    fd.sendto("hello", ainfo.resolve(addr, port).get_sockaddr());
    std::cout << "recv...\n";
    fd.recvfrom(5, remote);
    std::cout << "close...\n";
    fd.close();
}

void protobuf_test(const std::string &addr, const wU16 &port)
{
    wuk::net::WukSocket sock(AF_INET, SOCK_STREAM, 0);
    wuk::net::WukPacket packet;
    wuk::WukTime time;

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

void block_test(const std::string &addr, const wU16 &port)
{
    wuk::net::WukSocket fd(AF_INET, SOCK_STREAM, 0);

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

int main()
{
#   ifdef WUK_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    std::string remote_addr = "www.baidu.com";
    wU16 remote_port = 80;

    try {
        block_test(remote_addr, remote_port);
    } catch (wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
    }

#   ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
