#include <net/WukSocket.hh>
#include <utils/number.hh>
#include <WukMisc.hh>

#include <numeric>
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

static std::string receive_data(wuk::net::Socket &client)
{
    // 获取包长度
    std::string packet_length = client.recv_ex(4);
    if (packet_length.empty()) {
        return {};
    }

    // 解析包长度为数字
    wuk::u32 length = wuk::utils::unpack_bytes<wuk::u32>(packet_length);
    if (length == 0) {
        return {};
    }

    // 构建结果（初始化数据）
    std::string result;
    result.reserve(length); // 预分配空间，注意std::string result(128, '\0')不适用。

    while (length > 0) {
        wuk::u32 receive_size = wuk::min(2048U, length);
        std::string chunk = client.recv_ex(receive_size);
        if (chunk.empty()) {
            break;
        }
        result.append(chunk);
        length -= chunk.size();
    }

    return result;
}

struct Packet {
    wuk::u32 size = 0;
    wuk::u32 uid = 0;
    wuk::f64 timestamp = 0;
    std::string data;
    wuk::u32 crc = 0;
};

static Packet receive_data(wuk::net::Socket &client, int)
{
    // 获取包长度（仅包含第四项的长度，即数据流）
    std::string packet_length = client.recv_ex(4);
    if (packet_length.empty()) {
        return {};
    }
    
}

void server(wuk::f64 timeout = 15)
{
    // 处理新客户端连接的函数
    auto handle_new_client = \
    [](wuk::net::Socket &server, std::vector<wuk::net::Socket> &clients)
    {
        // 接受一个新的客户端连接
        wuk::net::Socket client = server.accept();
        // 将新客户端套接字设置为非阻塞模式
        client.set_blocking(false);

        // 从客户端列表中找到第一个无效的客户端套接字作为空槽位
        std::vector<wuk::net::Socket>::iterator \
        it = std::find_if(
            clients.begin(),
            clients.end(),
            [](const wuk::net::Socket &client)
            {
                return !client.is_valid();
            }
        );

        if (it != clients.end()) {
            // 如果找到了一个空槽位就将当前的客户端移动到此槽位
            *it = std::move(client);
        } else if (clients.size() < MAX_CLIENTS) {
            // 如果没找到空槽位且客户端队列还没满，那么扩充一个槽位
            clients.emplace_back(std::move(client));
        } else {
            // 如果没有空槽位并且客户端队列已满
            fmt::print("客户端队列数量已达上限，不应该接收新的连接。\n");
            return false;
        }

        // 返回对此客户端的引用（当前仅用于打印客户端信息）
        fmt::print("新的客户端连接：{0}:{1}。\n",
            (*it).get_raddr().get_address(),
            (*it).get_raddr().get_port()
        );
        return true;
    };

    // 处理客户端数据的函数
    auto handle_client_data = \
    [](wuk::net::Socket &client)
    {
        std::string data = receive_data(client);
        if (data.empty() || (data == "exit")) {
            fmt::print("客户端已断开连接：{0}:{1}。\n",
                client.get_raddr().get_address(),
                client.get_raddr().get_port()
            );
            client.shutdown(2);
            client.close();
            return;
        }
        fmt::print("客户端[{0}:{1}]，数据：{2}\n",
            client.get_raddr().get_address(),
            client.get_raddr().get_port(),
            data
        );
    };

    // 初始化服务端套接字
    fmt::print("初始化服务端套接字。\n");
    wuk::net::Socket server(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    server.set_timeout(timeout);
    server.set_blocking(false);
    server.setsockopt<wuk::i32>(SOL_SOCKET, SO_REUSEADDR, 1);
    server.bind("0.0.0.0", 48888);
    server.listen(MAX_CLIENTS);

    // 创建【读监听】【写监听】套接字集
    fd_set read_fds;
    fd_set write_fds;

    // 初始化客户端队列
    std::vector<wuk::net::Socket> clients(MAX_CLIENTS);

    fmt::print("循环开始。\n");
    for (wuk::u32 i = 0; i < 1000; ++i) {
        fmt::print("循环[{0:>4d}] 初始化套接字集\n", i+1);
        // 初始化【读监听】【写监听】套接字集
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        // 将服务端套接字加入到【读监听】队列
        FD_SET(server.get_fd(), &read_fds);

        fmt::print("循环[{0:>4d}] 初始化nfds\n", i+1);
        // 将当前的服务端套接字设置为最大套接字
        wuk::net::wSocket nfds = server.get_fd();
        // 遍历找出最大的套接字并赋值给nfds
        for (const wuk::net::Socket &client : clients) {
            if (!client.is_valid()) {
                // 如果套接字不是有效套接字就跳过
                continue;
            }
            // 将有效客户端套接字加入到【读监听】队列
            wuk::net::wSocket fd = client.get_fd();
            FD_SET(fd, &read_fds);
            // 如果当前客户端套接字比nfds更大，那么把nfds改为它
            if (fd > nfds) {
                nfds = fd;
            }
        }

        fmt::print("循环[{0:>4d}] 调用select函数\n", i+1);
        // 开始绑定select函数实现 IO多路复用
        timeval tv = create_timeval(timeout);
        wuk::i32 ready = select(nfds, &read_fds, &write_fds, nullptr, &tv);

        fmt::print("循环[{0:>4d}] 检测错误码\n", i+1);
        if (ready == 0) {
            // 套接字超时
            fmt::print("套接字超时了！走咯。\n");
            break;
        } else if (ready == -1) {
            // 套接字出错
            wuk::i32 code = wuk::net::err::system::code();
            std::string message = wuk::net::err::system::message(code);
            throw wuk::Exception(code, "server::select", message);
        }

        fmt::print("循环[{0:>4d}] 检查服务端套接字是否就绪\n", i+1);
        if (FD_ISSET(server.get_fd(), &read_fds)) {
            // 如果服务端套接字在【读监听】队列中被设置为了就绪就代表有新的连接。
            // 等于select告诉你可以开始调用accept来接受一个客户端了。
            // 将新的客户端添加到客户端队列中。
            if (!handle_new_client(server, clients)) {
                break;
            }
        }

        fmt::print("循环[{0:>4d}] 处理客户端数据\n", i+1);
        for (wuk::net::Socket &client : clients) {
            if (!client.is_valid() || !FD_ISSET(client.get_fd(), &read_fds)) {
                // 如果套接字无效或者未被标记为就绪，那么直接跳过
                continue;
            }
            handle_client_data(client);
        }

        fmt::print("循环[{0:>4d}] 清理资源\n", i+1);
        // 将所有有效套接字移至前方并将所有无效套接字标为垃圾并移至后方
        std::vector<wuk::net::Socket>::iterator \
        client_pos = std::remove_if(
            clients.begin(),
            clients.end(),
            [](const wuk::net::Socket &client)
            {
                return !client.is_valid();
            }
        );
        // 把所有堆在后方的垃圾清除掉以腾出空间
        clients.erase(client_pos, clients.end());
    }

    // 清理所有客户端连接
    for (wuk::net::Socket &client : clients) {
        if (!client.is_valid()) {
            continue;
        }
        client.shutdown(2);
        client.close();
    }
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
