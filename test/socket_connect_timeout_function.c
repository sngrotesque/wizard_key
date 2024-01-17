#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#if defined(__linux)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void connect_w_to(const char *target_addr, uint16_t target_port)
{
    int res;
    struct sockaddr_in addr;
    long arg;
    fd_set myset;
    struct timeval tv;
    int valopt;
    socklen_t lon;

    // Create socket
    int soc = socket(AF_INET, SOCK_STREAM, 0);
    if (soc < 0) {
        fprintf(stderr, "Error creating socket (%d %s)\n", errno, strerror(errno));
        exit(0);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(2000);
    addr.sin_addr.s_addr = inet_addr("192.168.0.1");

    // Set non-blocking
    if ((arg = fcntl(soc, F_GETFL, NULL)) < 0) {
        fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
        exit(0);
    }
    arg |= O_NONBLOCK;
    if (fcntl(soc, F_SETFL, arg) < 0) {
        fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
        exit(0);
    }
    // Trying to connect with timeout
    res = connect(soc, (struct sockaddr *)&addr, sizeof(addr));
    if (res < 0) {
        if (errno == EINPROGRESS) {
            fprintf(stderr, "EINPROGRESS in connect() - selecting\n");
            do {
                tv.tv_sec = 15;
                tv.tv_usec = 0;
                FD_ZERO(&myset);
                FD_SET(soc, &myset);
                res = select(soc + 1, NULL, &myset, NULL, &tv);
                if (res < 0 && errno != EINTR) {
                    fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
                    exit(0);
                } else if (res > 0) {
                    // Socket selected for write
                    lon = sizeof(int);
                    if (getsockopt(soc, SOL_SOCKET, SO_ERROR, (void *)(&valopt), &lon) < 0) {
                        fprintf(stderr, "Error in getsockopt() %d - %s\n", errno, strerror(errno));
                        exit(0);
                    }
                    // Check the value returned...
                    if (valopt) {
                        fprintf(stderr, "Error in delayed connection() %d - %s\n", valopt, strerror(valopt));
                        exit(0);
                    }
                    break;
                } else {
                    fprintf(stderr, "Timeout in select() - Cancelling!\n");
                    exit(0);
                }
            } while (1);
        } else {
            fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
            exit(0);
        }
    }
    // Set to blocking mode again...
    if ((arg = fcntl(soc, F_GETFL, NULL)) < 0) {
        fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
        exit(0);
    }
    arg &= (~O_NONBLOCK);
    if (fcntl(soc, F_SETFL, arg) < 0) {
        fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
        exit(0);
    }
    // I hope that is all
}
#elif defined(_WIN32)
#include <ws2tcpip.h>

void connect_w_to(const char *target_addr, uint16_t target_port)
{
    int res;
    struct sockaddr_in addr;
    unsigned long arg = 1;
    fd_set myset;
    struct timeval tv;
    int valopt;
    int lon;

    // Create socket
    SOCKET soc = socket(AF_INET, SOCK_STREAM, 0);
    if (soc == INVALID_SOCKET) {
        fprintf(stderr, "Error creating socket (%d)\n", WSAGetLastError());
        exit(0);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(target_port);
    addr.sin_addr.s_addr = inet_addr(target_addr);

    // Set non-blocking
    if (ioctlsocket(soc, FIONBIO, &arg) != 0) {
        fprintf(stderr, "Error ioctlsocket(..., FIONBIO) (%d)\n", WSAGetLastError());
        exit(0);
    }

    // Trying to connect with timeout
    res = connect(soc, (struct sockaddr *)&addr, sizeof(addr));
    if (res < 0) {
        if (WSAGetLastError() == WSAEWOULDBLOCK) {
            fprintf(stderr, "WSAEWOULDBLOCK in connect() - selecting\n");
            do {
                tv.tv_sec = 3;
                tv.tv_usec = 0;
                FD_ZERO(&myset);
                FD_SET(soc, &myset);
                res = select(0, NULL, &myset, NULL, &tv);
                if (res < 0 && WSAGetLastError() != WSAEINTR) {
                    fprintf(stderr, "Error connecting %d - %d\n", WSAGetLastError(), WSAGetLastError());
                    exit(0);
                } else if (res > 0) {
                    // Socket selected for write
                    lon = sizeof(int);
                    if (getsockopt(soc, SOL_SOCKET, SO_ERROR, (char *)(&valopt), &lon) != 0) {
                        fprintf(stderr, "Error in getsockopt() %d - %d\n", WSAGetLastError(), WSAGetLastError());
                        exit(0);
                    }
                    // Check the value returned...
                    if (valopt) {
                        fprintf(stderr, "Error in delayed connection() %d - %d\n", valopt, valopt);
                        exit(0);
                    }
                    break;
                } else {
                    fprintf(stderr, "Timeout in select() - Cancelling!\n");
                    exit(0);
                }
            } while (1);
        } else {
            fprintf(stderr, "Error connecting %d - %d\n", WSAGetLastError(), WSAGetLastError());
            exit(0);
        }
    }
    // Set to blocking mode again...
    arg = 0;
    if (ioctlsocket(soc, FIONBIO, &arg) != 0) {
        fprintf(stderr, "Error ioctlsocket(..., FIONBIO) (%d)\n", WSAGetLastError());
        exit(0);
    }
    // I hope that is all
    closesocket(soc);
}
#endif

int main()
{
#   ifdef _WIN32
    WSADATA ws;
    WSAStartup(MAKEWORD(2, 2), &ws);
#   endif

    connect_w_to("223.5.5.5", 8087);

#   ifdef _WIN32
    WSACleanup();
#   endif
    return 0;
}
