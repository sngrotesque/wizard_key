// python make.py test\main.cc -O3 -Wall -DWUK_EXPORTS -lws2_32 -lz
#include <config/WukConfig.hh>

#include <network/WukException.cc>
#include <network/WukSocket.cc>
#include <network/WukPacket.cc>

#include <WukPadding.cc>
#include <WukBuffer.cc>
#include <WukRandom.cc>
#include <WukMisc.cc>
#include <WukTime.cc>

#include <iostream>
#include <iomanip>
#include <memory>

using namespace std;

void SSS_WSAServiceControl(bool start)
{
    if(start) {
        WSADATA ws;
        WSAStartup(MAKEWORD(2,2), &ws);
    } else {
        WSACleanup();
    }
}

void WukBuffer_test()
{
    wuk::Buffer buffer;

    const char *text1 = "hello, world.\n";
    const char *text2 = "I'm SN-Grotesque.\n";
    const char *text3 = "I like C/C++/C#/Python.\n";
    const char *text4 = "testtesttesttesttesttesttest.\r\n";

    buffer.append((wByte *)text1, strlen(text1));
    buffer.append((wByte *)text2, strlen(text2));
    buffer.append((wByte *)text3, strlen(text3));
    memset(buffer.append_write(16), 0xff, 16);
    buffer.append((wByte *)text4, strlen(text4));

    printf("buffer data: %p\n", buffer.data);
    printf("buffer ofst: %p\n", buffer.data_offset);

    printf("buffer size: %zd\n", buffer.data_size);
    printf("buffer len:  %zd\n", buffer.data_len);

    wuk::misc::print_hex(buffer.get_data(), buffer.get_length(), 32, 1, 0);
    wuk::misc::print_pybytes(buffer.get_data(), buffer.get_length(), 1);
}

void NetworkPacket_BufferTest()
{
    SSS_WSAServiceControl(true);
    wuk::Buffer buffer{4096};

    wuk::net::Socket fd{AF_INET, SOCK_STREAM, IPPROTO_TCP};
    fd.connect("www.baidu.com", 80);
    fd.send("GET / HTTP/1.1\r\nHost: www.baidu.com\r\nUser-Agent: Android\r\n\r\n");

    ::recv(fd.fd, (char *)buffer.append_write(4096), 4096, 0);

    fd.close();

    cout << buffer.get_data() << endl;
    SSS_WSAServiceControl(false);
}

int main(int argc, char **argv)
{
    try {
        WukBuffer_test();
    } catch (exception &e) {
        cout << e.what() << endl;
    }

    return 0;
}
