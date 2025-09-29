#include <client/includes/WukAccount.hh>

#include <iostream>

int main()
{
#   ifdef WUK_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    try {
        wuk::im::client::Account acc("47.79.146.143", 47877);

        std::string username;
        std::string password;

        std::cout << "请输入用户名：";
        std::getline(std::cin, username);
        std::cout << "请输入密码：";
        std::getline(std::cin, password);

        acc.set_username(username);
        acc.set_password(password);

        acc.send_packet();
    } catch (wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

#   ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#   endif

    return 0;
}
