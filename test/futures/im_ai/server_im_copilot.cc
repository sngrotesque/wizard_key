// server.cpp
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include <cstdint>

#pragma comment(lib, "ws2_32.lib")

constexpr int PORT = 48888;
constexpr int TIMEOUT_SEC = 30;
constexpr int MAX_BUFFER = 4096;

struct ClientInfo {
    SOCKET socket;
    std::string username;
    std::string ip;
    uint16_t port;
};

std::string recv_packet(SOCKET sock) {
    char len_buf[4];
    int received = recv(sock, len_buf, 4, 0);
    if (received <= 0) return "";

    uint32_t msg_len = *reinterpret_cast<uint32_t*>(len_buf);
    std::string msg;
    while (msg.length() < msg_len) {
        char buffer[MAX_BUFFER];
        int chunk = recv(sock, buffer, std::min(MAX_BUFFER, (int)(msg_len - msg.length())), 0);
        if (chunk <= 0) return "";
        msg.append(buffer, chunk);
    }
    return msg;
}

void broadcast(const std::vector<ClientInfo>& clients, const std::string& sender, const std::string& timestamp, const std::string& message) {
    std::string full = "Client [" + sender + "] [" + timestamp + "]: " + message;
    uint32_t len = (uint32_t)full.size();
    std::string packet(reinterpret_cast<char*>(&len), 4);
    packet += full;

    for (const auto& client : clients) {
        send(client.socket, packet.data(), (int)packet.size(), 0);
    }
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, SOMAXCONN);
    u_long mode = 1;
    ioctlsocket(server_fd, FIONBIO, &mode);

    std::vector<ClientInfo> clients;
    fd_set read_fds;
    timeval timeout{TIMEOUT_SEC, 0};

    while (true) {
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);
        SOCKET max_fd = server_fd;

        for (const auto& client : clients) {
            FD_SET(client.socket, &read_fds);
            if (client.socket > max_fd) max_fd = client.socket;
        }

        int ready = select((int)max_fd + 1, &read_fds, nullptr, nullptr, &timeout);
        if (ready == 0) {
            std::cout << "Timeout reached. No connections. Shutting down.\n";
            break;
        }
        if (ready == SOCKET_ERROR) break;

        if (FD_ISSET(server_fd, &read_fds)) {
            sockaddr_in client_addr{};
            int len = sizeof(client_addr);
            SOCKET client_fd = accept(server_fd, (sockaddr*)&client_addr, &len);
            if (client_fd != INVALID_SOCKET) {
                ioctlsocket(client_fd, FIONBIO, &mode);
                std::string username = recv_packet(client_fd);
                if (username.empty()) {
                    closesocket(client_fd);
                    continue;
                }
                char ip_str[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, sizeof(ip_str));
                uint16_t port = ntohs(client_addr.sin_port);
                clients.push_back({client_fd, username, ip_str, port});
                std::cout << "New client connected: " << username << " [" << ip_str << ":" << port << "]\n";
            }
        }

        for (size_t i = 0; i < clients.size(); ) {
            SOCKET sock = clients[i].socket;
            if (FD_ISSET(sock, &read_fds)) {
                std::string msg = recv_packet(sock);
                if (msg.empty()) {
                    std::cout << "Client disconnected: " << clients[i].username << "\n";
                    closesocket(sock);
                    clients.erase(clients.begin() + i);
                    continue;
                }

                size_t sep = msg.find('|');
                std::string timestamp = msg.substr(0, sep);
                std::string content = msg.substr(sep + 1);

                std::cout << "Client [" << clients[i].username << "] [" << clients[i].ip << ":" << clients[i].port << "] [" << timestamp << "]: " << content << "\n";
                broadcast(clients, clients[i].username, timestamp, content);
            }
            ++i;
        }

        if (clients.empty()) {
            std::cout << "All clients disconnected. Shutting down.\n";
            break;
        }
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
