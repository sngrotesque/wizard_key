#include <net/WukSocket.hh>
#include <utils/bytes.hh>
#include <WukMisc.hh>

#include <functional>
#include <iostream>
#include <cmath>
#include <mutex>
#include <atomic>
#include <memory>
#include <algorithm>

using namespace wuk::misc;

static timeval create_timeval(wuk::f64 t)
{
    timeval tv{};
    wuk::f64 int_part;
    wuk::f64 float_part = std::modf(t, &int_part);

    tv.tv_sec = static_cast<time_t>(int_part);
    tv.tv_usec = static_cast<time_t>(float_part * 1e6);

    return tv;
}

#if 0
static void view_fd_set(const fd_set &fds)
{
    constexpr wuk::u32 block_size = 32;

    auto print_hex_easy = [](const void *p, wuk::ulong n) {
        const wuk::byte *buffer = reinterpret_cast<const wuk::byte *>(p);
        print_hex(buffer, n, block_size, n % block_size, true);
    };

    std::cout << "fd count:\n";
    print_hex_easy(&fds.fd_count, sizeof(fds.fd_count));

    std::cout << "fd array:\n";
    print_hex_easy(&fds.fd_array, (sizeof(fds.fd_array[0]) * fds.fd_count) + sizeof(fds.fd_array[0]));
}

static std::string recv_data(wuk::net::Socket &fd)
{
    std::string packet_length = fd.recv(4);
    wuk::i32 data_length {0};

    memcpy(&data_length, packet_length.data(), 4);
    wuk::i32 tmp_length = data_length;

    if (tmp_length == 0) {
        return {};
    }

    std::string result;
    while (tmp_length) {
        std::string tmp_data = fd.recv(wuk::min(2048, tmp_length));
        if (tmp_data.empty()) {
            break;
        }
        result += tmp_data;
        tmp_length -= tmp_data.length();
    }

    return result;
}

void server(wuk::f64 timeout = 15)
{
    wuk::net::Socket server_fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    fmt::print("初始化服务端套接字。\n");
    server_fd.set_blocking(false);
    server_fd.setsockopt<bool>(SOL_SOCKET, SO_REUSEADDR, true);
    server_fd.bind("0.0.0.0", 48888);
    server_fd.listen(3000);

    constexpr wuk::i32 MAX_CLIENTS = FD_SETSIZE;
    wuk::net::Socket client_fds[MAX_CLIENTS];

    for (wuk::u32 r = 0; r < 100; ++r) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server_fd.get_fd(), &read_fds);
        wuk::net::wSocket max_fd = server_fd.get_fd();

        fmt::print("初始化客户端套接字列表。\n");
        for (wuk::i32 i = 0; i < MAX_CLIENTS; ++i) {
            if (client_fds[i].is_valid()) {
                FD_SET(client_fds[i].get_fd(), &read_fds);
                if (client_fds[i].get_fd() > max_fd)
                    max_fd = client_fds[i].get_fd();
            }
        }

        fmt::print("绑定 IO 多路复用。\n");
        timeval timetv = create_timeval(timeout);
        wuk::i32 ready = select(max_fd + 1, &read_fds, nullptr, nullptr, &timetv);
        if (ready == 0) {
            fmt::print("套接字超时，退出。\n");
            break;
        } else if (ready == NETERROR) {
            wuk::i32 err_code = wuk::net::err::system::code();
            throw wuk::Exception(err_code, "select",
                wuk::net::err::system::message(err_code));
        }

        if (FD_ISSET(server_fd.get_fd(), &read_fds)) {
            wuk::net::Socket client_fd = server_fd.accept();
            client_fd.set_blocking(false);
            for (wuk::i32 i = 0; i < MAX_CLIENTS; ++i) {
                if (!client_fds[i].is_valid()) {
                    client_fds[i] = std::move(client_fd);
                    std::string client_addr = client_fds[i].get_raddr().get_address_string();
                    wuk::u16 client_port = client_fds[i].get_raddr().get_port();
                    std::cout << fmt::format("有新的客户端连接：{0}:{1}.\n",
                        client_addr, client_port);
                    break;
                }
            }
        }

        fmt::print("处理客户端数据！\n");
        for (wuk::i32 i = 0; i < MAX_CLIENTS; ++i) {
            wuk::net::Socket &client_fd = client_fds[i];
            if (client_fd.is_valid() && FD_ISSET(client_fd.get_fd(), &read_fds)) {
                std::string buffer = recv_data(client_fd);
                std::string client_addr = client_fd.get_raddr().get_address_string();
                wuk::u16 client_port = client_fd.get_raddr().get_port();
                if (buffer.empty() || buffer == "exit") {
                    std::cout << fmt::format("客户端断开连接：{0}:{1}.",
                        client_addr, client_port) << std::endl;
                    client_fd.shutdown(2);
                    client_fd.close();
                }
                std::cout << fmt::format("客户端 {0}:{1}, 接收到的数据：{2}",
                    client_addr, client_port, buffer) << std::endl;
            }
        }
    }

    server_fd.set_blocking(true);
    server_fd.close();
}
#endif

class ClientSession {
private:
    wuk::net::Socket m_fd;

public:
    ClientSession(wuk::net::Socket&& socket) 
        : m_fd(std::move(socket)) 
    {

    }

public:
    ClientSession(const ClientSession&) = delete;
    ClientSession(ClientSession&&) = default;
    ClientSession& operator=(const ClientSession&) = delete;
    ClientSession& operator=(ClientSession&&) = default;

public:
    bool is_valid() const
    {
        return m_fd.is_valid();
    }

    auto get_fd() const
    {
        return m_fd.get_fd();
    }

    std::string receive_data()
    {
        std::string packet_length = m_fd.recv(4);
        if (packet_length.empty()) {
            return {};
        }

        wuk::i32 data_length = wuk::utils::unpack_bytes<wuk::i32>(
            reinterpret_cast<wuk::byte *>(packet_length.data()),
            packet_length.capacity()
        );

        if (data_length == 0) return {};

        std::string result;
        wuk::i32 remaining = data_length;

        while (remaining > 0) {
            std::string chunk = m_fd.recv(std::min(2048, remaining));
            if (chunk.empty()) break;

            result += chunk;
            remaining -= chunk.length();
        }

        return result;
    }

    void disconnect()
    {
        m_fd.shutdown(2);
        m_fd.close();
    }

    std::string get_address() const
    {
        return m_fd.get_raddr().get_address_string();
    }

    wuk::u16 get_port() const
    {
        return m_fd.get_raddr().get_port();
    }
};

void server(wuk::f64 timeout = 15.0)
{
    wuk::net::Socket server_fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    fmt::println("初始化服务端套接字。");
    server_fd.set_blocking(false);
    server_fd.setsockopt<bool>(SOL_SOCKET, SO_REUSEADDR, true);
    server_fd.bind("0.0.0.0", 48888);
    server_fd.listen(3000);

    constexpr wuk::i32 MAX_CLIENTS = FD_SETSIZE;

    // 使用智能指针管理客户端会话
    std::vector<std::unique_ptr<ClientSession>> clients;
    clients.reserve(MAX_CLIENTS);

    // 使用lambda函数处理客户端连接
    auto handle_new_connection = [&]() {
        wuk::net::Socket client_fd = server_fd.accept();
        if (!client_fd.is_valid()) return;

        client_fd.set_blocking(false);

        // 查找空位或创建新位置
        auto it = std::find_if(
            clients.begin(), clients.end(),
            [](const std::unique_ptr<ClientSession> &client)
            {
                return !client || !client->is_valid();
            }
        );

        if (it != clients.end()) {
            *it = std::make_unique<ClientSession>(std::move(client_fd));
        } else if (clients.size() < MAX_CLIENTS) {
            clients.push_back(std::make_unique<ClientSession>(std::move(client_fd)));
        } else {
            fmt::println("客户端数量达到上限，拒绝新连接。");
            return;
        }

        fmt::println("有新的客户端连接：{0}:{1}。", 
            clients.back()->get_address(),
            clients.back()->get_port()
        );
    };

    // 使用lambda函数处理客户端数据[5](@ref)
    auto handle_client_data = [&](ClientSession& client)
    {
        std::string buffer = client.receive_data();
        if (buffer.empty() || buffer == "exit") {
            fmt::println("客户端断开连接：{0}:{1}",
                client.get_address(),
                client.get_port()
            );
            client.disconnect();
            return false; // 客户端断开
        }

        fmt::println("客户端 {0}:{1}, 接收到的数据：{2}",
            client.get_address(),
            client.get_port(),
            buffer
        );
        return true; // 客户端保持连接
    };

    for (wuk::u32 round = 0; round < 100; ++round) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server_fd.get_fd(), &read_fds);
        wuk::net::wSocket max_fd = server_fd.get_fd();

        // 设置有效的客户端fd
        for (const auto &client : clients) {
            if (client && client->is_valid()) {
                auto fd = client->get_fd();
                FD_SET(fd, &read_fds);
                if (fd > max_fd) max_fd = fd;
            }
        }

        fmt::println("绑定 IO 多路复用。");
        timeval timetv = create_timeval(timeout);
        wuk::i32 ready = select(max_fd + 1, &read_fds, nullptr, nullptr, &timetv);

        if (ready == 0) {
            fmt::println("套接字超时，退出。");
            break;
        } else if (ready == NETERROR) {
            wuk::i32 err_code = wuk::net::err::system::code();
            throw wuk::Exception(err_code, "select",
                wuk::net::err::system::message(err_code));
        }

        // 处理新连接
        if (FD_ISSET(server_fd.get_fd(), &read_fds)) {
            handle_new_connection();
        }

        // 处理客户端数据
        fmt::println("处理客户端数据！");
        for (auto& client : clients) {
            if (client && client->is_valid() && FD_ISSET(client->get_fd(), &read_fds)) {
                if (!handle_client_data(*client)) {
                    // 客户端断开，标记为无效
                    client.reset();
                }
            }
        }

        // 清理无效客户端
        clients.erase(
            std::remove_if(clients.begin(), clients.end(),
            [](const std::unique_ptr<ClientSession> &client)
            {
                return !client || !client->is_valid();
            }),
            clients.end());
    }

    // 清理所有客户端连接
    for (auto &client : clients) {
        if (client) {
            client->disconnect();
        }
    }

    fmt::println("服务器关闭。");
}

int main()
{
#   ifdef WUK_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2, 2), &ws);
#   endif

    try {
        server(30);
    } catch (const wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

#   ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
