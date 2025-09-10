#include <net/WukSocket.hh>
#include <WukMisc.hh>

#include <functional>
#include <iostream>
#include <cmath>
#include <zlib.h>

using namespace wuk::misc;

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

static timeval create_timeval(wuk::f64 t)
{
    timeval tv {0};

    wuk::f64 int_part;
    wuk::f64 float_part;

    float_part = modf(t, &int_part);

    tv.tv_sec = static_cast<time_t>(int_part);
    tv.tv_usec = static_cast<time_t>(float_part * 1e6);

    return tv;
}

static void view_fd_set(const fd_set &fds)
{
    constexpr wuk::u32 block_size = 32;

    auto print_hex_easy = [](const void *p, const wuk::ulong &n) {
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

    printf("TEST data length: %d\n", tmp_length);

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

void server()
{
    wuk::net::Socket server_fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    fd_set read_fds {0};

    server_fd.set_blocking(false);
    server_fd.setsockopt<bool>(SOL_SOCKET, SO_REUSEADDR, true);
    server_fd.bind("0.0.0.0", 48888);
    server_fd.listen(30);

    // 只为了测试，所以不应该让它循环太多次
    for (wuk::u32 r = 0; r < 10; ++r) {
        FD_ZERO(&read_fds);
        FD_SET(server_fd.get_fd(), &read_fds);

        std::cout << "socket fd: " << static_cast<int>(server_fd.get_fd()) << std::endl;

        view_fd_set(read_fds);

        timeval timetv = create_timeval(5);
        int ready = select(0, &read_fds, nullptr, nullptr, &timetv);
        if (ready == 0) {
            std::cout << "timeout, exit.\n";
            return;
        } else if (ready == NETERROR) {
            wuk::i32 err_code = wuk::net::err::system::code();
            throw wuk::Exception(err_code, "func",
                wuk::net::err::system::message(err_code));
        }

        if (FD_ISSET(server_fd.get_fd(), &read_fds) && server_fd.is_valid()) {
            auto client_fd = server_fd.accept();
            client_fd.set_blocking(true);
            auto client_addr = client_fd.get_raddr().get_address_string();
            auto client_port = client_fd.get_raddr().get_port();

            std::cout << "Client: " << client_addr << ":" << client_port << std::endl;
            while (true) {
                std::string buffer = recv_data(client_fd);
                if (buffer == "exit" || buffer.empty()) {
                    std::cerr << "Client close.\n";
                    client_fd.shutdown(2);
                    client_fd.close();
                    break;
                }

                std::cout << "Client: "
                          << client_addr << ":" << client_port
                          <<  ", recv buffer: "
                          << buffer << std::endl;
            }
        }

        view_fd_set(read_fds);
    }

    server_fd.set_blocking(true);
    server_fd.close();
}

void server(int)
{
    wuk::net::Socket server_fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    server_fd.set_blocking(false);
    server_fd.setsockopt<bool>(SOL_SOCKET, SO_REUSEADDR, true);
    server_fd.bind("0.0.0.0", 48888);
    server_fd.listen(30);

    constexpr int MAX_CLIENTS = FD_SETSIZE;
    wuk::net::Socket client_fds[MAX_CLIENTS];

    for (wuk::u32 r = 0; r < 100; ++r) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server_fd.get_fd(), &read_fds);
        wuk::net::wSocket max_fd = server_fd.get_fd();

        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (client_fds[i].is_valid()) {
                FD_SET(client_fds[i].get_fd(), &read_fds);
                if (client_fds[i].get_fd() > max_fd)
                    max_fd = client_fds[i].get_fd();
            }
        }

        timeval timetv = create_timeval(5);
        int ready = select(max_fd + 1, &read_fds, nullptr, nullptr, &timetv);
        if (ready == 0) {
            std::cout << "timeout, exit.\n";
            break;
        } else if (ready == NETERROR) {
            wuk::i32 err_code = wuk::net::err::system::code();
            throw wuk::Exception(err_code, "select", wuk::net::err::system::message(err_code));
        }

        // 新连接
        if (FD_ISSET(server_fd.get_fd(), &read_fds)) {
            auto client_fd = server_fd.accept();
            client_fd.set_blocking(false);
            for (int i = 0; i < MAX_CLIENTS; ++i) {
                if (!client_fds[i].is_valid()) {
                    client_fds[i] = std::move(client_fd);
                    std::cout << "New client: " << client_fds[i].get_raddr().get_address_string()
                              << ":" << client_fds[i].get_raddr().get_port() << std::endl;
                    break;
                }
            }
        }

        // 客户端数据
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            auto &fd = client_fds[i];
            if (fd.is_valid() && FD_ISSET(fd.get_fd(), &read_fds)) {
                std::string buffer = recv_data(fd);
                if (buffer.empty() || buffer == "exit") {
                    std::cerr << "Client disconnected: "
                              << fd.get_raddr().get_address_string()
                              << ":" << fd.get_raddr().get_port() << std::endl;
                    fd.shutdown(2);
                    fd.close();
                } else {
                    std::cout << "Client: "
                              << fd.get_raddr().get_address_string()
                              << ":" << fd.get_raddr().get_port()
                              << ", recv buffer: " << buffer << std::endl;
                }
            }
        }
    }

    server_fd.set_blocking(true);
    server_fd.close();
}

// python make.py test\futures\futures.cc -DWUK_EXPORTS -lws2_32

int main()
{
#   ifdef WUK_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2, 2), &ws);
#   endif

    try {
        server(1);
    } catch (const wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
    }

#   ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
