#include <network/socket.hpp>

#include <iostream>
#include <fstream>

#include <network/socket.cpp>
#include <network/exception.cpp>

using namespace std;

namespace wmkc_test {
    class PacketBuffer {
    public:
        wByte *buffer;
        wU32 length;

        PacketBuffer() {}
    };

    void listen(string listen_addr, uint16_t listen_port) {
        wmkc::net::Socket listen_fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        int enable = 1;
        setsockopt(listen_fd.fd, SOL_SOCKET, SO_REUSEADDR, (char *)&enable, sizeof(enable));
        listen_fd.bind(listen_addr, listen_port);
        listen_fd.listen(5);

        printf("waiting client connected...\n");
        auto client_fd = listen_fd.accept();
        printf("client connected.\n");

        string string_buffer = client_fd.recv(4);
        string buffer{};
        wU32 length{};

        memcpy(&length, string_buffer.data(), 4);

        printf("start recv...\n");
        while(length) {
            string temp = client_fd.recv(wmkc_min(2048, length));
            if(temp.empty()) {
                break;
            }
            buffer.append(temp);
            length -= temp.size();
        }

        printf("file writing...\n");
        fstream f("C:/Users/sn/Desktop/f_tp_test/test", ios::out | ios::binary);
        f.write(buffer.c_str(), buffer.size());
        f.close();

        client_fd.send("done.");

        client_fd.close();
        listen_fd.close();
    }
}

int main()
{
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);

    wmkc_test::listen("0.0.0.0", 80);

    WSACleanup();
}
