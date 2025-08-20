#include <iostream>

#include <thread>
#include <vector>
#include <memory>

#include <string_view>
#include <unordered_map>
#include <optional>
#include <atomic>
#include <mutex>

#include <net/WukSocket.hh>

struct ClientInfo {
    std::string username;
    std::string address;
};

std::unordered_map<wuk::net::wSocket, ClientInfo> clients;
std::atomic<bool> server_active{true};
std::mutex clients_mutex;

static void log(const std::string &msg)
{
    std::cout << msg << std::endl;
}

static void fd_send(wuk::net::Socket &fd, const std::string &data)
{
    wU32 len = htonl(static_cast<wU32>(data.size()));
    char length_array[sizeof(wU32)] = {0};
    memcpy(length_array, &len, sizeof(wU32));

    fd.send(std::string(length_array, 4));
    fd.send(data);
}

static std::optional<std::string> fd_recv(wuk::net::wSocket sock)
{
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

static void broadcast(wuk::net::Socket &fd, const std::string &message)
{
    std::lock_guard lock(clients_mutex);
    for (const auto& [sock, _] : clients) {
        if (sock != fd.get_fd()) {
            fd_send(fd, message);
        }
    }
}

static void handle_client(wuk::net::Socket server_fd, wuk::net::Socket fd, std::string addr_str)
{
    auto remove_client = [&]() -> void {
        std::lock_guard lock(clients_mutex);
        auto it = clients.find(fd.get_fd());
        if (it != clients.end()) {
            log("[断开连接] 用户 " + it->second.username + " (" + it->second.address + ") 已退出");
            clients.erase(it);
            if (clients.empty() && server_active) {
                log("所有用户已断开，服务器即将关闭...");
                server_active = false;
                // 此处会触发10004号错误代码。
                server_fd.close();
            } else {
                fd.close();
            }
        }
    };

    auto username_opt = fd_recv(fd.get_fd());
    if (!username_opt) {
        remove_client();
        return;
    }

    do {
        std::lock_guard lock(clients_mutex);
        clients[fd.get_fd()] = ClientInfo{*username_opt, addr_str};
    } while (0);

    log("[新连接] 用户 " + *username_opt + " (" + addr_str + ") 加入聊天室");
    broadcast(fd, "系统通知: " + *username_opt + " 进入了聊天室");

    while (true) {
        auto msg_opt = fd_recv(fd.get_fd());
        if (!msg_opt || msg_opt->empty() || *msg_opt == "exit" || *msg_opt == "quit") break;
        broadcast(fd, "[" + *username_opt + "] " + *msg_opt);
    }

    remove_client();
}

static void start_server(const std::string &host = "0.0.0.0", wU16 port = 47777)
{
    wuk::net::Socket server_fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    server_fd.setsockopt<bool>(SOL_SOCKET, SO_REUSEADDR, true);
    server_fd.bind(host, port);
    server_fd.listen(5);

    log("服务器已启动；监听：" + host + ":" + std::to_string(port));

    while (server_active) {
        auto client = server_fd.accept();
        std::string client_addr = client.get_laddr().get_address_string();
        std::thread(handle_client, server_fd, client, client_addr).detach();
    }

    log("服务器已关闭。");
}

int main() {
#ifdef WUK_PLATFORM_WINOS
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    try {
        start_server();
    } catch (wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

#ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#endif
    return 0;
}
