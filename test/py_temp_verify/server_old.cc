#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <optional>
#include <atomic>
#include <vector>
#include <sstream>
#include <memory>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    using socket_t = SOCKET;
    #define CLOSESOCKET closesocket
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    using socket_t = int;
    #define INVALID_SOCKET (-1)
    #define CLOSESOCKET close
#endif

constexpr int PORT = 47777;
constexpr int BACKLOG = 5;

struct ClientInfo {
    std::string username;
    std::string address;
};

std::unordered_map<socket_t, ClientInfo> clients;
std::mutex clients_mutex;
std::atomic<bool> server_active{true};
socket_t global_server_socket = INVALID_SOCKET;

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
    while (total_received < len) {
        int chunk = recv(sock, &buffer[total_received], static_cast<int>(len - total_received), 0);
        if (chunk <= 0) return std::nullopt;
        total_received += chunk;
    }
    return buffer;
}

void broadcast(socket_t sender, std::string_view message) {
    std::lock_guard lock(clients_mutex);
    for (const auto& [sock, _] : clients) {
        if (sock != sender) {
            fd_send(sock, message);
        }
    }
}

void remove_client(socket_t sock) {
    std::lock_guard lock(clients_mutex);
    auto it = clients.find(sock);
    if (it != clients.end()) {
        log("[断开连接] 用户 " + it->second.username + " (" + it->second.address + ") 已退出");
        clients.erase(it);
        CLOSESOCKET(sock);
        if (clients.empty() && server_active) {
            log("所有用户已断开，服务器即将关闭...");
            CLOSESOCKET(global_server_socket);
            server_active = false;
        }
    }
}

void handle_client(socket_t client_sock, std::string addr_str) {
    auto username_opt = fd_recv(client_sock);
    if (!username_opt) {
        remove_client(client_sock);
        return;
    }

    {
        std::lock_guard lock(clients_mutex);
        clients[client_sock] = ClientInfo{*username_opt, addr_str};
    }

    log("[新连接] 用户 " + *username_opt + " (" + addr_str + ") 加入聊天室");
    broadcast(client_sock, "系统通知: " + *username_opt + " 进入了聊天室");

    while (true) {
        auto msg_opt = fd_recv(client_sock);
        if (!msg_opt || msg_opt->empty() || *msg_opt == "exit" || *msg_opt == "quit") break;
        broadcast(client_sock, "[" + *username_opt + "] " + *msg_opt);
    }

    remove_client(client_sock);
}

void start_server(const std::string& host = "0.0.0.0", int port = PORT) {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_UNSPEC;        // 支持 IPv4 和 IPv6
    hints.ai_socktype = SOCK_STREAM;    // TCP
    hints.ai_flags = AI_PASSIVE;        // 用于 bind 的服务器地址

    std::string port_str = std::to_string(port);
    int status = getaddrinfo(host.c_str(), port_str.c_str(), &hints, &res);
    if (status != 0 || !res) {
        log("getaddrinfo 失败: " + std::string(gai_strerror(status)));
        return;
    }

    global_server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (global_server_socket == INVALID_SOCKET) {
        log("无法创建套接字");
        freeaddrinfo(res);
        return;
    }

    int opt = 1;
    setsockopt(global_server_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&opt), sizeof(opt));

    if (bind(global_server_socket, res->ai_addr, static_cast<int>(res->ai_addrlen)) < 0) {
        log("绑定失败");
        freeaddrinfo(res);
        return;
    }

    freeaddrinfo(res); // 绑定后即可释放

    if (listen(global_server_socket, BACKLOG) < 0) {
        log("监听失败");
        return;
    }

    log("服务器已启动，监听 " + host + ":" + port_str);

    while (server_active) {
        sockaddr_storage client_addr{};
        socklen_t addr_len = sizeof(client_addr);
        socket_t client_sock = accept(global_server_socket, reinterpret_cast<sockaddr*>(&client_addr), &addr_len);
        if (client_sock == INVALID_SOCKET) break;

        char ip_str[INET6_ADDRSTRLEN];
        void* addr_ptr = nullptr;
        if (client_addr.ss_family == AF_INET) {
            addr_ptr = &reinterpret_cast<sockaddr_in*>(&client_addr)->sin_addr;
        } else if (client_addr.ss_family == AF_INET6) {
            addr_ptr = &reinterpret_cast<sockaddr_in6*>(&client_addr)->sin6_addr;
        }

        inet_ntop(client_addr.ss_family, addr_ptr, ip_str, sizeof(ip_str));
        std::string addr_str = std::string(ip_str) + ":" + std::to_string(ntohs(reinterpret_cast<sockaddr_in*>(&client_addr)->sin_port));

        std::thread(handle_client, client_sock, addr_str).detach();
    }

    log("服务器已关闭");

#ifdef _WIN32
    WSACleanup();
#endif
}

int main() {
    start_server();
    return 0;
}