#include <net/WukSocket.hh>
#include <utils/number.hh>
#include <crypto/WukHash.hh>

#include <filesystem>
#include <fstream>

void normal_test()
{
    wuk::net::Socket server(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    fmt::print("初始化服务端套接字。\n");
    server.setsockopt<wuk::i32>(SOL_SOCKET, SO_REUSEADDR, true);
    server.set_timeout(15);
    server.bind("0.0.0.0", 48888);
    server.listen(5);

    fmt::print("等待客户端连接。\n");
    wuk::net::Socket client = server.accept();
    fmt::print("客户端已连接：{}:{}。\n",
        client.get_raddr().get_address(),
        client.get_raddr().get_port()
    );

    wuk::i32 needed = wuk::utils::unpack_bytes<wuk::i32>(client.recv(4));
    fmt::print("总计需要接收 {:.4f} MB的数据。\n",
        (static_cast<wuk::f64>(needed) / 1024 / 1024)
    );

    wuk::Buffer message = client.recvall(needed);
    fmt::print("已接收的数据长度：{:.4f} MB。\n",
        (static_cast<wuk::f64>(message.size()) / 1024 / 1024)
    );

    wuk::crypto::Hashlib<wuk::crypto::HashlibType::SHA_256> hash;
    hash.update(message);
    std::string hexdigest = hash.hexdigest();

    fmt::print("数据哈希（SHA-256）：{}。\n", hexdigest);

    std::filesystem::path path(R"(C:\Users\sn\Desktop\code_test\bg_test.jpg)");
    std::fstream f(path, std::ios::binary | std::ios::out);

    f.write(message.c_str(), message.size());

    client.close();
    server.close();
}

void cyberpunk_test()
{
    wuk::net::Socket server(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    fmt::print("初始化服务端套接字。\n");
    server.setsockopt<wuk::i32>(SOL_SOCKET, SO_REUSEADDR, true);
    server.set_timeout(15);
    server.bind("0.0.0.0", 48888);
    server.listen(5);

    fmt::print("等待客户端连接。\n");
    wuk::net::Socket client = server.accept();
    fmt::print("客户端已连接：{}:{}。\n",
        client.get_raddr().get_address(),
        client.get_raddr().get_port()
    );

    wuk::i32 needed = wuk::utils::unpack_bytes<wuk::i32>(client.recv(4));
    fmt::print("总计需要接收 {:.4f} MB的数据。\n",
        (static_cast<wuk::f64>(needed) / 1024 / 1024)
    );

    wuk::Buffer message(needed);
    wuk::i32 remaining = needed;
    while (remaining >= 4096) {
        message += client.recv(4096);
        remaining -= 4096;
    }
    if (remaining > 0) {
        message += client.recv(remaining);
    }

    std::filesystem::path path("/root/code/bg_test.jpg");
    std::fstream f(path, std::ios::binary | std::ios::out);

    f.write(message.c_str(), message.size());

    client.close();
    server.close();
}

int main()
{
#   ifdef WUK_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    try {
        // normal_test();
        cyberpunk_test();
    } catch (const wuk::Exception &e) {
        fmt::print("{}\n", e.what());
        return 1;
    }

#   ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
