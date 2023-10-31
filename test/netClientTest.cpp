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

void net_client_test()
{
    wmkcNet *sockfd = new wmkcNet(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    wmkc_u32 data_length = 16777216;
    wmkcNetBufT *data = new wmkcNetBufT[data_length];
    wmkcNetBufT *p = data;

    sockfd->connect("127.0.0.1", 49281);
    while(data_length) {
        sockfd->recv(p, data_length, 0);
        data_length -= sockfd->size;
        p += sockfd->size;
    }
    for(wmkc_u32 x = 0; x < 16777216; ++x) {
        if(data[x] != 0) {
            cout << "Error." << endl;
        }
    }
    sockfd->close();
    delete sockfd;
}

int main(int argc, char **argv)
{
#   ifdef WMKC_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    net_client_test();

#   ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
