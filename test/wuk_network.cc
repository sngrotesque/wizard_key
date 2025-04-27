#include <config/WukConfig.hh>
#include <config/WukException.hh>

#include <network/WukSocket.cc>
#include <network/WukIPEndPoint.cc>
#include <network/WukSocketOptions.cc>
#include <WukBuffer.cc>

#include <iostream>
using namespace std;

#define TEST_SERVER_HOST_V4 "0.0.0.0"
#define TEST_CLIENT_HOST_V4 "127.0.0.1"

#define TEST_SERVER_HOST_V6 "::"
#define TEST_CLIENT_HOST_V6 "::1"

#define TEST_COMM_PORT 49981

namespace udp {
    void wuknet_server_test()
    {
        wuk::net::Socket fd{AF_INET6, SOCK_DGRAM, IPPROTO_UDP};
        wuk::net::IPEndPoint client;

        wuk::Buffer recvBuffer;
        wuk::Buffer sendBuffer{"test server message."};

        fd.setsockopt(SOL_SOCKET, SO_REUSEADDR, wuk::net::SockOpt{1});
        fd.bind(TEST_SERVER_HOST_V6, TEST_COMM_PORT);

        recvBuffer = fd.recvfrom(4096, client);

        cout << recvBuffer.get_cstr() << endl;

        fd.sendto(sendBuffer, client);

        fd.shutdown(wuk::net::SD_SW::BOTH);
        fd.close();
    }

    void wuknet_client_test()
    {
        wuk::net::Socket fd{AF_INET, SOCK_DGRAM, IPPROTO_UDP};
        wuk::net::IPEndPoint pointer{TEST_CLIENT_HOST_V4, TEST_COMM_PORT};

        wuk::Buffer recvBuffer;
        wuk::Buffer sendBuffer{"test client message."};

        fd.sendto(sendBuffer, pointer);

        recvBuffer = fd.recvfrom(256, pointer);
        cout << recvBuffer.get_cstr() << endl;

        fd.shutdown(wuk::net::SD_SW::BOTH);
        fd.close();
    }
}

namespace tcp {
    void wuknet_server_host()
    {
        wuk::net::Socket fd{AF_INET, SOCK_STREAM, IPPROTO_TCP};
        
        wuk::Buffer recvBuffer;
        wuk::Buffer sendBuffer{"test server message."};

        fd.setsockopt(SOL_SOCKET, SO_REUSEADDR, wuk::net::SockOpt{1});
        fd.bind(TEST_SERVER_HOST_V4, TEST_COMM_PORT);
        fd.listen(5);

        cout << "wait connected..." << endl;
        wuk::net::Socket client_fd = fd.accept();

        client_fd.shutdown(wuk::net::SD_SW::BOTH);

        client_fd.close();
        fd.close();
    }
}

void with_python()
{
    wuk::net::Socket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    fd.connect("www.baidu.com", 80);

    cout << "socket fd: " << fd.get_fileno() << endl;
    // system("pause");

    fd.close();
}

int main()
{
#   ifdef WUK_PLATFORM_WINOS
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2,2), &ws)) {
        throw runtime_error("WSAStartup error!" + to_string(WSAGetLastError()));
    }
#   endif

    try {
        // tcp::wuknet_server_host();
        with_python();
    } catch (wuk::Exception &e) {
        cout << "Error! " << e.what() << endl;
    } catch (wuk::net::Exception &e) {
        cout << "Net Error! " << e.what() << endl;
    }

#   ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
