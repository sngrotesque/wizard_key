#include <iostream>
#include <string>
#include <string_view>
#include <thread>
#include <atomic>
#include <optional>
#include <vector>
#include <cstring>
#include <algorithm>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    using socket_t = SOCKET;
    #define CLOSESOCKET closesocket
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    using socket_t = int;
    #define INVALID_SOCKET (-1)
    #define CLOSESOCKET close
#endif

constexpr int PORT = 47777;
std::atomic<bool> stop_flag{false};

void log(std::string_view msg) {
    std::cout << msg << std::endl;
}

void fd_send(socket_t sock, std::string_view data) {
    uint32_t len = htonl(static_cast<uint32_t>(data.size()));
    send(sock, reinterpret_cast<const char*>(&len), sizeof(len), 0);
    send(sock, data.data(), static_cast<int>(data.size()), 0);
}

std::optional<std::string> fd_recv(socket_t sock) {
    uint32_t len = 0;
    int received = recv(sock, reinterpret_cast<char*>(&len), sizeof(len), 0);
    if (received <= 0) return std::nullopt;
    len = ntohl(len);

    std::string buffer(len, '\0');
    size_t total_received = 0;
    while (total_received < len && !stop_flag.load()) {
        int chunk = recv(sock, &buffer[total_received], static_cast<int>(len - total_received), 0);
        if (chunk <= 0) return std::nullopt;
        total_received += chunk;
    }
    return buffer;
}

void receive_messages(socket_t client_sock) {
    while (!stop_flag.load()) {
        auto msg_opt = fd_recv(client_sock);
        if (!msg_opt || msg_opt->empty()) {
            log("\n与服务器的连接已断开");
            stop_flag = true;
            break;
        }
        std::cout << "\n" << *msg_opt << "\nEnter message: ";
        std::cout.flush();
    }
}

socket_t connect_to_server(const std::string& host, int port) {
    addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    std::string port_str = std::to_string(port);
    int status = getaddrinfo(host.c_str(), port_str.c_str(), &hints, &res);
    if (status != 0 || !res) {
        log("getaddrinfo 失败: " + std::string(gai_strerror(status)));
        return INVALID_SOCKET;
    }

    socket_t sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == INVALID_SOCKET) {
        log("无法创建套接字");
        freeaddrinfo(res);
        return INVALID_SOCKET;
    }

    if (connect(sock, res->ai_addr, static_cast<int>(res->ai_addrlen)) < 0) {
        log("无法连接到服务器");
        CLOSESOCKET(sock);
        freeaddrinfo(res);
        return INVALID_SOCKET;
    }

    freeaddrinfo(res);
    return sock;
}

void start_client() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    std::string username;
    std::cout << "请输入您的用户名: ";
    std::getline(std::cin, username);

    socket_t client_sock = connect_to_server("127.0.0.1", PORT);
    if (client_sock == INVALID_SOCKET) return;

    fd_send(client_sock, username);

    std::thread receiver(receive_messages, client_sock);

    const std::vector<std::string> quit_commands = {"exit", "quit", "dc"};
    while (!stop_flag.load()) {
        std::string message;
        std::cout << "Enter message: ";
        std::getline(std::cin, message);

        std::string lower_msg = message;
        std::transform(lower_msg.begin(), lower_msg.end(), lower_msg.begin(),
                    [](unsigned char c) { return std::tolower(c); });

        if (std::find(quit_commands.begin(), quit_commands.end(), lower_msg) != quit_commands.end()) {
            fd_send(client_sock, message);
            stop_flag = true;
            break;
        }

        fd_send(client_sock, message);
    }

    receiver.join();
    CLOSESOCKET(client_sock);

#ifdef _WIN32
    WSACleanup();
#endif
}
int main() {
    start_client();
    return 0;
}
