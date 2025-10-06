/** 说明 ********************
 * -- 函数声明 ---------------------------------------------------------------
 * 
 * int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, const TIMEVAL *timeout);
 * 
 * -- 参数 ---------------------------------------------------------------
 * 
 * Platform:           Windows
 * Reference: https://learn.microsoft.com/zh-cn/windows/win32/api/winsock2/nf-winsock2-select
 * 
 * [in] nfds           已忽略。 包含 nfds 参数只是为了与 Berkeley 套接字兼容。
 * [in, out] readfds   一个可选指针，指向要检查的一组套接字的可读性。
 * [in, out] writefds  指向要检查可写性的一组套接字的可选指针。
 * [in, out] exceptfds 指向要检查错误的一组套接字的可选指针。
 * [in] timeout        选择等待的最长时间，以 TIMEVAL 结构的形式提供。 将阻止操作的 超时 参数设置为 null 。
 * 
 * Platform:           Linux
 * Reference: https://man7.org/linux/man-pages/man2/select.2.html
 * 
 * [in] nfds           需要监视的最大文件描述符加1，即待监视的文件描述符的最大值加1。
 * [in, out] readfds   一个可选指针，指向要检查的一组套接字的可读性。
 * [in, out] writefds  指向要检查可写性的一组套接字的可选指针。
 * [in, out] exceptfds 指向要检查错误的一组套接字的可选指针。
 * [in] timeout        选择等待的最长时间，以 TIMEVAL 结构的形式提供。 将阻止操作的 超时 参数设置为 null 。
 * 
 * -- 返回值 ---------------------------------------------------------------
 * 
 * >=0：返回当前有事件发生的文件描述符的总数
 * ==0：表示超时，没有事件发生，不是错误。
 * <=0：出错，可使用全局错误代码查看代码。
 * 
 * -- 操作API ---------------------------------------------------------------
 * 
 * FD_ZERO：初始化为空集（使用之前都应初始化）
 * FD_CLR：从集中删除套接字
 * FD_ISSET：检查是否为SET的成员，如果是将返回True
 * FD_SET：添加要设置的套接字
 * 
*/
#include <net/WukSocket.hh>
#include <utils/bytes.hh>
#include <WukMisc.hh>

#include <iostream>
#include <cmath>

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

    while (true) {
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
                    std::string client_addr = client_fds[i].get_raddr().get_address();
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
                std::string client_addr = client_fd.get_raddr().get_address();
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
