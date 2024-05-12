#include "header.hpp"

int main(int argc, char **argv)
{
#   ifdef WMKC_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    wmkc::net::Socket fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    fd.bind(_FTP_ADDR, _FTP_PORT);
    fd.listen(5);

    wmkc::net::Socket cfd = fd.accept();

    cout << cfd.recv(4096) << endl;
    cfd.send("over.\n");

    cfd.close();
    fd.close();

#   ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
