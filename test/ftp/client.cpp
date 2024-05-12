#include "header.hpp"

int main(int argc, char **argv)
{
#   ifdef WMKC_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    wmkc::net::Socket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    fd.connect(_FTP_ADDR, _FTP_PORT);
    fd.send("hello, world. this is test.");
    cout << fd.recv(32) << endl;

    fd.close();

#   ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
