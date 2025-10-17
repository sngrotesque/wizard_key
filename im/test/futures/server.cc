#include <common/includes/proto/login_request.pb.h>
#include <common/includes/proto/login_response.pb.h>
#include <net/WukSocket.hh>
#include <db/WukPsql.hh>
#include <utils/number.hh>

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

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

static wuk::Buffer receive_buffer(wuk::net::Socket &fd, wuk::i32 length)
{
    wuk::Buffer result(length);

    while (length > 0) {
        wuk::Buffer temp = fd.recv_ex(length);
        if (temp.empty()) {
            break;
        }
        result += temp;
        length -= temp.size();
    }

    return result;
}

void server(wuk::f64 timeout = 15)
{
    // 初始化服务端套接字
    fmt::print("初始化服务端套接字。\n");

    std::string binding_addr("0.0.0.0");
    wuk::u16 binding_port(48888);

    wuk::net::Socket server(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    server.set_timeout(timeout * 2);
    server.set_blocking(false);
    server.setsockopt<wuk::i32>(SOL_SOCKET, SO_REUSEADDR, 1);
    server.setsockopt<wuk::i32>(IPPROTO_TCP, TCP_NODELAY, 1);
    server.bind(binding_addr, binding_port);
    server.listen(MAX_CLIENTS);

    // 初始化服务端数据库连接
    wuk::db::psql::Connection conn(
        "host=localhost "
        "port=5432 "
        "user=postgres "
        "password=123456 "
        "dbname=im"
    );
    wuk::db::psql::Work work(std::move(conn));

    // 创建【读监听】【写监听】套接字集
    fd_set read_fds;
    fd_set write_fds;

    // 初始化客户端队列
    std::vector<wuk::net::Socket> clients(MAX_CLIENTS);

    fmt::print("循环开始，绑定至{}:{}。\n", binding_addr, binding_port);
    while (true) {
        // 初始化【读监听】【写监听】套接字集
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        // 将服务端套接字加入到【读监听】队列
        FD_SET(server.get_fd(), &read_fds);

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

        // 开始绑定select函数实现 IO多路复用
        timeval tv = create_timeval(timeout);
        wuk::i32 ready = select(nfds + 1, &read_fds, &write_fds, nullptr, &tv);

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

        if (FD_ISSET(server.get_fd(), &read_fds)) {
            // 如果服务端套接字在【读监听】队列中被设置为了就绪就代表有新的连接。
            // 等于select告诉你可以开始调用accept来接受一个客户端了。
            // 将新的客户端添加到客户端队列中。

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
            }

            // 返回对此客户端的引用（当前仅用于打印客户端信息）
            const wuk::net::Sockaddr &client_info = (*it).get_raddr();
            fmt::print("新的客户端连接：{0}:{1}。\n",
                client_info.get_address(),
                client_info.get_port()
            );
        }

        for (wuk::net::Socket &client : clients) {
            if (!client.is_valid() || !FD_ISSET(client.get_fd(), &read_fds)) {
                // 如果套接字无效或者未被标记为就绪，那么直接跳过
                continue;
            }

            // 处理客户端内容
            wuk::i32 packet_size = wuk::utils::unpack_bytes<wuk::i32>(client.recv_ex(4));
            
        }

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
