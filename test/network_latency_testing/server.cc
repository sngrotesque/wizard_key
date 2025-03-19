#include "header.hh"

int main()
{
#   ifdef WUK_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif
    try {
        wuk::net::Socket server_fd{AF_INET, SOCK_STREAM, IPPROTO_TCP};
        wuk::net::Socket client_fd;
        wuk::net::SockOpt opt{1};
        double timeout_val{3};

        server_fd.setsockopt(SOL_SOCKET, SO_REUSEADDR, opt);
        server_fd.settimeout(timeout_val);
        server_fd.bind("0.0.0.0", PORT);
        server_fd.listen(3);

        std::cout << "Server is listening on port: " << PORT << std::endl;
        client_fd = server_fd.accept();

        std::cout << "The client has been connected." << std::endl;
        auto start_time = std::chrono::high_resolution_clock::now();
        for(wSize total_bytes = 300 * 4096; total_bytes; total_bytes -= client_fd.get_transmission_length()) {
            wuk::Buffer buffer = client_fd.recv(BUFFER_SIZE);
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

        std::cout << "Data transfer completed in "
                << static_cast<double>(duration / 1e6) - timeout_val
                << " microseconds"
                << std::endl;

        client_fd.close();
        server_fd.close();
    } catch (wuk::Exception &e) {
        std::cout << e.what() << std::endl;
    } catch (wuk::net::Exception &e) {
        std::cout << e.what() << std::endl;
    }

#   ifdef WUK_PLATFORM_WINOS
    WSACleanup();
#   endif
    return 0;
}

