#include <net/WukSocket.hh>
#include <utils/number.hh>
#include <crypto/WukHash.hh>

#include <filesystem>
#include <fstream>

void normal_test()
{
    std::filesystem::path path(R"(C:\Users\sn\Desktop\code_test\bg.jpg)");
    std::streamsize size = std::filesystem::file_size(path);
    std::fstream f(path, std::ios::binary | std::ios::in);

    if (!f) {
        fmt::print("{} 文件打开失败。\n", path.string());
        return;
    }
    fmt::print("文件大小：{:.4f} MB。\n",
        (static_cast<wuk::f64>(size) / 1024 / 1024)
    );

    wuk::Buffer message(size);
    f.read(message.write<char>(size), size);

    wuk::crypto::Hashlib<wuk::crypto::HashlibType::SHA_256> hash;
    hash.update(message);
    std::string hexdigest = hash.hexdigest();

    fmt::print("数据哈希（SHA-256）：{}。\n", hexdigest);

    /////////////////////////////////////////////////////
    std::string addr("localhost");
    wuk::u16 port(48888);

    wuk::net::Socket client(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    client.set_timeout(15);
    client.connect(addr, port);

    fmt::print("已连接服务器。\n");

    client.send(wuk::utils::pack_bytes<wuk::i32>(size));
    fmt::print("已将文件大小发送至服务器。\n");

    client.sendall(message);
    fmt::print("数据已发送完毕。\n");

    client.close();
}

int main()
{
#   ifdef WUK_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    try {
        normal_test();
    } catch (const wuk::Exception &e) {
        fmt::print("{}\n", e.what());
        return 1;
    }

#   ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}
