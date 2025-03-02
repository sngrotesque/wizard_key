#include "header.hh"

int main()
{
#   ifdef WUK_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif
    try {
        wuk::net::Socket fd{AF_INET, SOCK_STREAM, IPPROTO_TCP};
        wSize bytes_sent = 0;

        fd.connect("47.243.162.23", PORT);
        // fd.connect("localhost", PORT);

        wByte _buffer_temp[4096];
        wuk::Buffer buffer{_buffer_temp, sizeof(_buffer_temp)};

        for(wSize i = 0; i < 300; ++i) {
            fd.send(buffer);
            bytes_sent += fd.get_transmission_length();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << "Total bytes sent: " << bytes_sent << std::endl;

        fd.close();
    } catch (wuk::Exception &e) {
        std::cout << e.what() << std::endl;
    } catch (wuk::net::Exception &e) {
        std::cout << e.what() << std::endl;
    }

#   ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#   endif
}
