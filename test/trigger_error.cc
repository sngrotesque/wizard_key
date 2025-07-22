#include <config/WukConfig.hh>
#include <network/WukSocket.hh>
#include <network/WukException.hh>

#include <iostream>

void close_fd(wSocket fd)
{
#   ifdef WUK_PLATFORM_WINOS
    closesocket(fd);
#   else
    close(fd);
#   endif
}

void PrintLastError()
{
#   ifdef WUK_PLATFORM_WINOS
    DWORD error_code = WSAGetLastError();
    char* error_msg = nullptr;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error_code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&error_msg,
        0,
        NULL
    );

    std::cerr << "Error code: " << error_code << " - " << error_msg << std::endl;

    LocalFree(error_msg);
#   else
    int error_code = errno;
    std::cerr << "Error code: " << error_code << " - " << strerror(error_code) << std::endl;
#   endif
}

int main() {
    wSocket sock = socket(99991, SOCK_STREAM, IPPROTO_TCP);
    if (sock == EOF) {
        PrintLastError();
    }

    // 记得关闭套接字
    close_fd(sock);

    return 0;
}

