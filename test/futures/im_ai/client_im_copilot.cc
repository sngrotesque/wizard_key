// client.cpp
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

std::string make_packet(const std::string& msg) {
    uint32_t len = (uint32_t)msg.size();
    std::string packet(reinterpret_cast<char*>(&len), 4);
    packet += msg;
    return packet;
}

std::string current_timestamp() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto sec = duration_cast<seconds>(now.time_since_epoch()).count();
    auto usec = duration_cast<microseconds>(now.time_since_epoch()).count() % 1000000;
    return std::to_string(sec) + "." + std::to_string(usec);
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(48888);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    connect(sock, (sockaddr*)&server, sizeof(server));

    std::string username;
    std::cout << "Enter username: ";
    std::getline(std::cin, username);

    std::string user_packet = make_packet(username);
    send(sock, user_packet.data(), (int)user_packet.size(), 0);

    fd_set read_fds;
    while (true) {
        FD_ZERO(&read_fds);
        FD_SET(sock, &read_fds);
        FD_SET(0, &read_fds); // stdin

        timeval timeout{1, 0};
        int ready = select(0 + 1, &read_fds, nullptr, nullptr, &timeout);
        if (ready > 0) {
            if (FD_ISSET(0, &read_fds)) {
                std::string msg;
                std::getline(std::cin, msg);
                std::string full = current_timestamp() + "|" + msg;
                std::string packet = make_packet(full);
                send(sock, packet.data(), (int)packet.size(), 0);
            }
            if (FD_ISSET(sock, &read_fds)) {
                char len_buf[4];
                int r = recv(sock, len_buf, 4, 0);
                if (r <= 0) break;
                uint32_t len = *reinterpret_cast<uint32_t*>(len_buf);
                std::string msg;
                while (msg.length() < len) {
                    char buf[1024];
                    int chunk = recv(sock, buf, std::min(1024u, len - (uint32_t)msg.length()), 0);
                    if (chunk <= 0) break;
                    msg.append(buf, chunk);
                }
                std::cout << msg << "\n";
            }
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
