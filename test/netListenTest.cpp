#include <fstream>
#include <stdexcept>

#include <iostream>
#include <filesystem>
#include <format>
#include <tuple>

#include <network/wmkc_net.c>
#include <network/wmkc_ssl.c>
#include <crypto/snc.c>
#include <wmkc_memory.c>
#include <wmkc_struct.c>
#include <wmkc_random.c>
#include <wmkc_basic.c>
#include <wmkc_file.c>
#include <wmkc_misc.c>
#include <wmkc_time.c>
#include <wmkc_pad.c>

// #include <network/wmkc_net.hpp>
#include <network/wmkc_net.cpp>

void net_listen_test()
{
    wmkcNet *sockfd = new wmkcNet(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    wmkcNet *cSockfd = wmkcNull;
    sockfd->bind("0.0.0.0", 49281);
    sockfd->listen(3);
    cout << "等待客户端的连接..." << endl;
    cSockfd = sockfd->accept();

    wmkcNetBufT *data = new wmkcNetBufT[16777216];
    wmkcMem_zero(data, 16777216);
    cSockfd->sendall(data, 16777216);

    cSockfd->close();
    sockfd->close();
    delete[] data;
    delete cSockfd;
    delete sockfd;
}

int main(int argc, char **argv)
{
#   ifdef WMKC_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    net_listen_test();

#   ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
