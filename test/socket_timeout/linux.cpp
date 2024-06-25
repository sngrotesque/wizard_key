#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#include <sys/time.h>

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <stdexcept>

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
    int fd;
    SocketType(int family, int type, int proto)
    : family(family), type(type), proto(proto)
    {
        this->fd = socket(family, type, proto);
        if(this->fd == EOF) {
            throw_exception("SocketType::SocketType", errno, "Failed tp create socket.");
        }
    }
};

struct timeval create_timeval(double number)
{
    struct timeval tv{};
    double intPart, floatPart;
    floatPart = modf(number, &intPart);
    tv.tv_sec = (time_t)intPart;
    tv.tv_usec = (time_t)floatPart;
    return tv;
}

void Connect(SocketType Socket, std::string remote_addr, uint16_t remote_port, double timeout)
{
    struct addrinfo hints{
        .ai_family = Socket.family,
        .ai_socktype = Socket.type,
        .ai_protocol = Socket.proto};
    struct addrinfo *result{};

    int arg;
    fd_set my_fd_set;
    struct timeval tv = create_timeval(timeout);

    if(getaddrinfo(remote_addr.c_str(), std::to_string(remote_port).c_str(), &hints, &result)) {
        throw_exception("Connect", errno, strerror(errno));
    }

    if((arg = fcntl(Socket.fd, F_GETFL, nullptr)) == EOF) {
        throw_exception("Connect", errno, strerror(errno));
    }
    if(fcntl(Socket.fd, F_SETFL, (arg |= O_NONBLOCK)) == EOF) {
        throw_exception("Connect", errno, strerror(errno));
    }

    int err_code;
    if(connect(Socket.fd, result->ai_addr, result->ai_addrlen) == EOF) {
        if(errno == EINPROGRESS) {
            printf("EINPROGRESS in connect() - selecting.\n");
            for(;;) {
                FD_ZERO(&my_fd_set);
                FD_SET(Socket.fd, &my_fd_set);

                err_code = select(Socket.fd + 1, nullptr, &my_fd_set, nullptr, &tv);
                if((err_code == EOF) && (errno != EINTR)) {
                    throw_exception("Connect", errno, strerror(errno));
                } else if(err_code) {
                    int optVal;
                    socklen_t optVal_len;

                    optVal_len = sizeof(optVal);

                    err_code = getsockopt(Socket.fd, SOL_SOCKET, SO_ERROR, (void *)&optVal, &optVal_len);
                    if(err_code == EOF) {
                        throw_exception("Connect", errno, strerror(errno));
                    }

                    if(optVal) {
                        throw_exception("Connect", errno, strerror(errno));
                    }
                    break;
                } else {
                    throw_exception("Connect", errno, strerror(errno));
                }
            }
        } else {
            throw_exception("Connect", errno, strerror(errno));
        }
    }

    if((arg = fcntl(Socket.fd, F_GETFL, nullptr)) == EOF) {
        throw_exception("Connect", errno, strerror(errno));
    }
    if(fcntl(Socket.fd, F_SETFL, (arg &= (~O_NONBLOCK))) == EOF) {
        throw_exception("Connect", errno, strerror(errno));
    }
}

int main()
{
    try {
        SocketType fd{AF_INET, SOCK_STREAM, IPPROTO_TCP};
        Connect(fd, "112.45.122.107", 4444, 5.0);
        char send_buffer[] = {
            "GET / HTTP/1.1\r\n"
            "Host: passport.bilibili.com\r\n"
            "Connection: close\r\n"
            "Accept: */*\r\n"
            "User-Agent: Android\r\n"
            "\r\n"};
        char recv_buffer[4096]{};
        
        if(send(fd.fd, send_buffer, strlen(send_buffer), 0) == EOF) {
            throw_exception("main", errno, strerror(errno));
        }
        if(recv(fd.fd, recv_buffer, sizeof(recv_buffer) - 1, 0) == EOF) {
            throw_exception("main", errno, strerror(errno));
        }

        printf("%s\n", recv_buffer);

    } catch (std::exception &e) {
        printf("Error! %s\n", e.what());
    }

    return 0;
}
