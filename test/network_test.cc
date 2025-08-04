#include <net/WukSocket.hh>
#include <net/WukPacket.hh>

#include <iostream>
#ifdef WUK_STD_CPP_20
#   include <format>
#else
#   include <sstream>
#endif

void server_test(const std::string &addr, const wU16 &port)
{
    try {
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
    } catch (wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void client_test(const std::string &addr, const wU16 &port)
{
    try {
        wuk::net::WukSocket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        fd.connect(addr, port);

#       ifdef WUK_STD_CPP_20
        std::string headers = std::format(
            "GET / HTTP/1.1\r\n"
            "Host: {0}\r\n"
            "Accept: */*\r\n"
            "User-Agent: Android\r\n\r\n",
            addr
        );
#       else
        std::stringstream ss;
        ss  << "GET / HTTP/1.1\r\n"
            << "Host: " << addr << "\r\n"
            << "Accept: */*\r\n"
            << "User-Agent: Android\r\n\r\n";
        std::string headers = ss.str();
#       endif
        fd.send(headers);

        std::cout << fd.recv(4096) << std::endl;

        fd.close();
    } catch (wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

int main()
{
#   ifdef WUK_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    std::string remote_addr = "www.baidu.com";
    wU16 remote_port = 80;

    client_test(remote_addr, remote_port);

#   ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
