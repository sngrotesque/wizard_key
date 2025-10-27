#include <net/WukSocket.hh>
#include <utils/number.hh>
#include <WukMisc.hh>

#include <functional>
#include <iostream>
#include <cmath>
#include <mutex>
#include <atomic>
#include <memory>
#include <algorithm>

using namespace wuk::misc;

constexpr wuk::i32 MAX_CLIENTS = FD_SETSIZE;

static timeval create_timeval(wuk::f64 t)
{
    timeval tv{};
    wuk::f64 int_part;
    wuk::f64 float_part = std::modf(t, &int_part);

    tv.tv_sec = static_cast<time_t>(int_part);
    tv.tv_usec = static_cast<time_t>(float_part * 1e6);

    return tv;
}

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

    auto fd() const
    {
        return m_fd.fd();
    }

    std::string receive_data()
    {
        std::string packet_length = m_fd.recv(4);
        if (packet_length.empty()) {
            return {};
        }

        wuk::i32 data_length = wuk::utils::unpack_bytes<wuk::i32>(packet_length);

        if (data_length == 0) {
            return {};
        }

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
        return m_fd.get_remote().get_address();
    }

    wuk::u16 get_port() const
    {
        return m_fd.get_remote().get_port();
    }
};

void server(wuk::f64 timeout = 15.0)
{
    // 处理新连接的函数
    auto handle_new_connection = [](wuk::net::Socket &server_fd, std::vector<ClientSession> &clients)
    {
        // 接受新连接
        wuk::net::Socket client_fd = server_fd.accept();
        // 将新连接的客户端设置为非阻塞
        client_fd.set_blocking(false);

        // 从序列中，找到第一个符合条件的元素
        auto it = std::find_if(
            clients.begin(), clients.end(),
            [](const ClientSession &client)
            {
                // 返回一个已释放的套接字资源（用于复用资源）
                // 此处理解为空槽位。
                return !client.is_valid();
            }
        );

        if (it != clients.end()) {
            // 如果找到了一个空槽位
            *it = ClientSession(std::move(client_fd));
        } else if (clients.size() < MAX_CLIENTS) {
            // 如果没找到空槽位，且整个槽位还没达到数量限制，扩充一个槽位
            clients.emplace_back(std::move(client_fd));
        } else {
            // 如果没找到空槽位并且已达到数量限制。
            fmt::println("客户端数量达到上限，拒绝新连接。");
            return;
        }

        // 返回对最后一个元素的引用（在此仅用于打印客户端信息）
        const auto &client = clients.back();
        fmt::println("有新的客户端连接：{0}:{1}。",
            client.get_address(), client.get_port());
    };

    // 处理客户端数据的函数
    auto handle_client_data = [](ClientSession &client) {
        std::string buffer = client.receive_data();
        if (buffer.empty() || buffer == "exit") {
            // 如果客户端发送了空消息和“exit”视为断开连接。
            fmt::println("客户端断开连接：{0}:{1}",
                client.get_address(),
                client.get_port()
            );
            client.disconnect();
            return false;
        }
        // 打印客户端发送的消息
        fmt::println("客户端 {0}:{1}, 接收到的数据：{2}",
            client.get_address(), client.get_port(), buffer);
        return true;
    };

    // 初始化服务端套接字
    fmt::println("初始化服务端套接字。");
    wuk::net::Socket server_fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // 将服务端套接字设置为非阻塞模式
    server_fd.set_blocking(false);
    // 将服务端套接字设置为可复用地址
    server_fd.setsockopt<bool>(SOL_SOCKET, SO_REUSEADDR, true);
    server_fd.bind("0.0.0.0", 48888);
    server_fd.listen(3000);

    // 初始化客户端队列
    std::vector<ClientSession> clients;
    clients.reserve(MAX_CLIENTS);

    fmt::println("开始等待客户端连接。");
    while (true) {
        // 创建套接字集
        fd_set read_fds;
        // 初始化（清空套接字集）
        FD_ZERO(&read_fds);
        // 将服务端套接字加入到【读监听】队列
        FD_SET(server_fd.fd(), &read_fds);
        // 将max_fd设置为当前的服务端套接字文件描述符的数字
        wuk::net::wSocket max_fd = server_fd.fd();

#       ifndef WUK_COMPILER_MSVC
        // 遍历找出套接字文件描述符数字最大的那一个并赋值给max_fd
        for (const auto &client : clients) {
            if (client.is_valid()) {
                // 如果当前元素是有效的套接字
                auto fd = client.fd();
                // 将此客户端套接字加入【读监听】队列
                FD_SET(fd, &read_fds);
                // 如果它的套接字文件描述符更大，那么将它赋值给max_fd
                if (fd > max_fd) max_fd = fd;
            }
        }
#       endif

        // fmt::println("绑定 IO 多路复用。");
        timeval timetv = create_timeval(timeout);
        wuk::i32 ready = select(max_fd + 1, &read_fds, nullptr, nullptr, &timetv);

        if (ready == 0) {
            // select函数返回值为0代表超时
            fmt::println("套接字超时，退出。");
            break;
        } else if (ready == NETERROR) {
            // 返回值为-1代表出错
            wuk::i32 err_code = wuk::net::err::system::code();
            throw wuk::Exception(err_code, "select",
                wuk::net::err::system::message(err_code));
        }

        if (FD_ISSET(server_fd.fd(), &read_fds)) {
            // 如果服务端套接字在【读监听】队列中就绪就代表有新的客户端连接
            // 将新客户端连接到客户端队列
            handle_new_connection(server_fd, clients);
        }

        // fmt::println("处理客户端数据！");
        for (auto &client : clients) {
            if (!client.is_valid() || !FD_ISSET(client.fd(), &read_fds)) {
                continue;
            }
            handle_client_data(client);
        }

        // 使用std::remove_if将所有有效套接字移到客户端队列前面并将无效套接字标记为垃圾且移动到队列后方
        auto client_pos = std::remove_if(
            clients.begin(), clients.end(),
            [](const ClientSession &client)
            {
                return !client.is_valid();
            }
        );
        // 把所有堆在尾部的垃圾套接字清理掉腾出空间
        clients.erase(client_pos, clients.end());
    }

    // 最后如果套接字超时，断开与所有有效客户端套接字的连接
    for (auto &client : clients) {
        if (client.is_valid()) {
            client.disconnect();
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
        return 1;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

#   ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
