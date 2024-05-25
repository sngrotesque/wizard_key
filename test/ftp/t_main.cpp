#include "ftp.hpp"

#include "ftp.cpp"

#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
#   ifdef WMKC_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    ftp_send("F:/fix_btrfs.txt", "223.5.5.5", 53);

#   ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
