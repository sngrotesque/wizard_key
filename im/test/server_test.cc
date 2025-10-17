// [windows] python py\exec.py im\test\server_test.cc -lws2_32 -DWUK_DEBUG
// [linux]   python py/exec.py im/test/server_test.cc -DWUK_DEBUG
#include <server/includes/WukServer.hh>

#include <iostream>

int main()
{
#   ifdef WUK_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    try {
        wuk::im::server::Server server(48888);

        server.init();
        server.run();
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
