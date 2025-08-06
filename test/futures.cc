#include <stdexcept>

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <fcntl.h>
    #include <unistd.h>
    #include <errno.h>
#endif

namespace wuk::net {

    // 函数A：设置套接字为阻塞模式
    void set_blocking(int socket_fd) {
    #ifdef _WIN32
        u_long mode = 0; // 0 = 阻塞模式
        if (ioctlsocket(socket_fd, FIONBIO, &mode) != NO_ERROR) {
            throw std::runtime_error("Failed to set blocking mode on Windows");
        }
    #else
        int flags = fcntl(socket_fd, F_GETFL, 0);
        if (flags == -1) {
            throw std::runtime_error("fcntl(F_GETFL) failed");
        }
        if (fcntl(socket_fd, F_SETFL, flags & ~O_NONBLOCK) == -1) {
            throw std::runtime_error("fcntl(F_SETFL) failed to set blocking mode");
        }
    #endif
    }

    // 函数B：设置套接字为非阻塞模式
    void set_nonblocking(int socket_fd) {
    #ifdef _WIN32
        u_long mode = 1; // 1 = 非阻塞模式
        if (ioctlsocket(socket_fd, FIONBIO, &mode) != NO_ERROR) {
            throw std::runtime_error("Failed to set non-blocking mode on Windows");
        }
    #else
        int flags = fcntl(socket_fd, F_GETFL, 0);
        if (flags == -1) {
            throw std::runtime_error("fcntl(F_GETFL) failed");
        }
        if (fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
            throw std::runtime_error("fcntl(F_SETFL) failed to set non-blocking mode");
        }
    #endif
    }

}
