#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <WS2Tcpip.h>

void throw_exception(const char *function, const int code, const char *content)
{
    char message[128]{};
    snprintf(message, sizeof(message), "%s[%d]: %s", function, code, content);
    throw std::runtime_error(message);
}

class SocketType {
public:
    int family;
    int type;
    int proto;
    SOCKET fd;
    SocketType(int family, int type, int proto)
    : family(family), type(type), proto(proto)
    {
        this->fd = socket(family, type, proto);
        if(this->fd == SOCKET_ERROR) {
            throw_exception("SocketType::SocketType", WSAGetLastError(), "Failed tp create socket.");
        }
    }
};

TIMEVAL create_timeval(double number)
{
    TIMEVAL tv{};
    double intPart, floatPart;
    floatPart = modf(number, &intPart);
    tv.tv_sec = (long)intPart;
    tv.tv_usec = (long)floatPart;
    return tv;
}

void Connect(SocketType Socket, std::string remote_addr, uint16_t remote_port, double timeout)
{
    ADDRINFO hints{
                .ai_family = Socket.family,
                .ai_socktype = Socket.type,
                .ai_protocol = Socket.proto};
    ADDRINFO *result{};

    u_long mode = true; // 阻塞模式启用与否
    fd_set my_fd_set; // fd_set用于将套接字放入“集”中以实现各种目的
    TIMEVAL tv = create_timeval(timeout); // 设置套接字超时时间

    if(getaddrinfo(remote_addr.c_str(), std::to_string(remote_port).c_str(), &hints, &result)) {
        throw_exception("Connect", WSAGetLastError(), "Failed to call getaddrinfo.");
    }

    if(ioctlsocket(Socket.fd, FIONBIO, &mode)) {
        throw_exception("Connect", WSAGetLastError(), "Failed to call ioctlsocket[1].");
    }

    int err_code;
    if(connect(Socket.fd, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR) {
        if(WSAGetLastError() == WSAEWOULDBLOCK) {
            printf("WSAEWOULDBLOCK in connect() - selecting.\n");
            for(;;) {
                FD_ZERO(&my_fd_set);
                FD_SET(Socket.fd, &my_fd_set);

                err_code = select(0, nullptr, &my_fd_set, nullptr, &tv);
                if((err_code == SOCKET_ERROR) && (WSAGetLastError() != WSAEINTR)) {
                    printf("Error: %d\n", WSAGetLastError());
                    throw_exception("Connect", WSAGetLastError(), "Failed to call select.");
                } else if(err_code) {
                    int optVal;
                    int optVal_len;

                    optVal_len = sizeof(optVal);

                    err_code = getsockopt(Socket.fd, SOL_SOCKET, SO_ERROR, (char *)&optVal, &optVal_len);
                    if(err_code == SOCKET_ERROR) {
                        printf("Error: %d\n", WSAGetLastError());
                        throw_exception("Connect", WSAGetLastError(), "Failed to call getsockopt.");
                    }

                    if(optVal) {
                        throw_exception("Connect", optVal, "Error in delayed connection.");
                    }
                    break;
                } else {
                    throw_exception("Connect", 0, "The call to select timed out.");
                }
            }
        } else {
            printf("Error: %d\n", WSAGetLastError());
            throw_exception("Connect", WSAGetLastError(), "Failed to connecting.");
        }

        // 为什么将这个放在connect域里面？不是很理解。
        mode = false;
        if(ioctlsocket(Socket.fd, FIONBIO, &mode)) {
            throw_exception("Connect", WSAGetLastError(), "Failed to call ioctlsocket[2].");
        }
    }
}

int main()
{
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);

    try {
        SocketType fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        Connect(fd, "112.45.122.107", 4444, 5.0);
        char send_buffer[] = {
            "GET / HTTP/1.1\r\n"
            "Host: passport.bilibili.com\r\n"
            "Connection: close\r\n"
            "Accept: */*\r\n"
            "User-Agent: Android\r\n"
            "\r\n"};
        char recv_buffer[4096]{};

        if(send(fd.fd, send_buffer, strlen(send_buffer), 0) == SOCKET_ERROR) {
            throw_exception("main", WSAGetLastError(), "Failed to call send.");
        }
        if(recv(fd.fd, recv_buffer, sizeof(recv_buffer) - 1, 0) == SOCKET_ERROR) {
            throw_exception("main", WSAGetLastError(), "Failed to call recv.");
        }

        printf("%s\n", recv_buffer);

    } catch (std::exception &e) {
        printf("Error! %s\n", e.what());
    }

    WSACleanup();
    return 0;
}
