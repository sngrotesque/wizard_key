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

#include <network/wmkc_net.cpp>

wmkcBool check_chcp()
{
    if(GetConsoleOutputCP() != 65001) {
        if(SetConsoleOutputCP(65001)) {
            return true;
        }
    } else {
        return true;
    }
    return false;
}

#define TARGET_ADDR "www.baidu.com"

int main(int argc, char **argv)
{
    check_chcp();
#   ifdef WMKC_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    wmkcNet *sockfd = new wmkcNet(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockfd->connect(TARGET_ADDR, 80);
    sockfd->send(
        "GET / HTTP/1.1\r\n"
        "Host: " TARGET_ADDR "\r\n"
        "Accept: */*\r\n"
        "Connection: close\r\n"
        "User-Agent: Android\r\n\r\n"
    );
    cout << sockfd->recv(4096) << endl;

    sockfd->shutdown(2);
    sockfd->close();
#   ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
