#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <thread>
#include <atomic>

#ifdef _MSC_VER
#   pragma comment(lib, "ws2_32.lib")
#endif

#define BUFFER_SIZE 4096

std::string get_current_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration - seconds);
    
    return std::to_string(seconds.count()) + "." + std::to_string(microseconds.count());
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <port> <username>" << std::endl;
        return 1;
    }

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(static_cast<u_short>(std::stoi(argv[2])));

    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    // Send username first
    std::string username = argv[3];
    send(client_socket, username.c_str(), username.size(), 0);

    std::cout << "Connected to server as " << username << std::endl;
    std::cout << "Type your messages (type 'exit' to quit):" << std::endl;

    fd_set readfds;
    std::atomic<bool> running(true);

    while (running) {
        FD_ZERO(&readfds);
        FD_SET(client_socket, &readfds);
        FD_SET(STDIN_FILENO, &readfds);

        timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int activity = select(0, &readfds, nullptr, nullptr, &timeout);
        if (activity == SOCKET_ERROR) {
            std::cerr << "Select error: " << WSAGetLastError() << std::endl;
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            std::string input;
            std::getline(std::cin, input);
            
            if (input == "exit") {
                running = false;
                break;
            }

            // Prepare message with timestamp
            std::string timestamp = get_current_timestamp();
            std::string message = timestamp + ":" + input;
            
            // Send message length first (4 bytes, little-endian)
            uint32_t msg_length = static_cast<uint32_t>(message.size());
            // No need to convert to network byte order (requirement #9)
            send(client_socket, (const char*)&msg_length, sizeof(msg_length), 0);
            
            // Then send the actual message
            send(client_socket, message.c_str(), message.size(), 0);
        }

        if (FD_ISSET(client_socket, &readfds)) {
            // First read the message length (4 bytes)
            uint32_t msg_length;
            int bytes_received = recv(client_socket, (char*)&msg_length, sizeof(msg_length), 0);
            
            if (bytes_received <= 0) {
                std::cerr << "Server disconnected" << std::endl;
                running = false;
                break;
            }

            // Convert from network byte order to host byte order (not needed as per requirement #9)
            // msg_length = ntohl(msg_length);

            // Then read the actual message
            std::vector<char> buffer(msg_length + 1);
            bytes_received = recv(client_socket, buffer.data(), msg_length, 0);
            
            if (bytes_received <= 0) {
                std::cerr << "Error receiving message from server" << std::endl;
                running = false;
                break;
            }

            buffer[bytes_received] = '\0';
            std::cout << buffer.data() << std::endl;
        }
    }

    closesocket(client_socket);
    WSACleanup();

    return 0;
}