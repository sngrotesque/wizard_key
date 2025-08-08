#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#ifdef _MSC_VER
#   pragma comment(lib, "ws2_32.lib")
#endif

#define MAX_CLIENTS 64
#define BUFFER_SIZE 4096
#define TIMEOUT_SECONDS 30

struct ClientInfo {
    SOCKET socket;
    std::string username;
    std::string ip_port;
    sockaddr_in address;
};

std::string get_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration - seconds);
    
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "." 
        << std::setfill('0') << std::setw(6) << microseconds.count();
    
    return oss.str();
}

std::string get_current_timestamp_for_message() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration - seconds);
    
    return std::to_string(seconds.count()) + "." + std::to_string(microseconds.count());
}

std::string get_ip_port(const sockaddr_in& addr) {
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);
    return std::string(ip) + ":" + std::to_string(ntohs(addr.sin_port));
}

void broadcast_message(const std::vector<ClientInfo>& clients, SOCKET sender, const std::string& message) {
    for (const auto& client : clients) {
        if (client.socket != INVALID_SOCKET && client.socket != sender) {
            send(client.socket, message.c_str(), message.size(), 0);
        }
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server started on port 8080 at " << get_timestamp() << std::endl;

    std::vector<ClientInfo> clients(MAX_CLIENTS);
    for (auto& client : clients) {
        client.socket = INVALID_SOCKET;
    }

    timeval timeout;
    timeout.tv_sec = TIMEOUT_SECONDS;
    timeout.tv_usec = 0;

    bool running = true;
    while (running) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server_socket, &readfds);

        SOCKET max_sd = server_socket;
        int active_clients = 0;

        for (const auto& client : clients) {
            if (client.socket != INVALID_SOCKET) {
                FD_SET(client.socket, &readfds);
                if (client.socket > max_sd) {
                    max_sd = client.socket;
                }
                active_clients++;
            }
        }

        if (active_clients == 0) {
            std::cout << "No active clients, waiting for new connections with timeout " 
                      << TIMEOUT_SECONDS << " seconds..." << std::endl;
        }

        int activity = select(0, &readfds, nullptr, nullptr, &timeout);
        if (activity == SOCKET_ERROR) {
            std::cerr << "Select error: " << WSAGetLastError() << std::endl;
            break;
        }

        if (activity == 0) {
            std::cout << "Timeout reached with no connections, shutting down..." << std::endl;
            break;
        }

        if (FD_ISSET(server_socket, &readfds)) {
            sockaddr_in client_addr;
            int addr_len = sizeof(client_addr);
            SOCKET new_socket = accept(server_socket, (sockaddr*)&client_addr, &addr_len);
            if (new_socket == INVALID_SOCKET) {
                std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
                continue;
            }

            // Find empty slot for new client
            bool slot_found = false;
            for (auto& client : clients) {
                if (client.socket == INVALID_SOCKET) {
                    client.socket = new_socket;
                    client.address = client_addr;
                    client.ip_port = get_ip_port(client_addr);
                    
                    // Receive username first
                    char username_buffer[256];
                    int username_len = recv(new_socket, username_buffer, sizeof(username_buffer), 0);
                    if (username_len <= 0) {
                        closesocket(new_socket);
                        client.socket = INVALID_SOCKET;
                        std::cerr << "Failed to receive username from new client" << std::endl;
                        break;
                    }
                    
                    client.username = std::string(username_buffer, username_len);
                    std::cout << "New connection from " << client.ip_port 
                              << " with username: " << client.username << " at " 
                              << get_timestamp() << std::endl;
                    
                    // Notify all clients about new user
                    std::string join_msg = "Client " + client.username + " joined the chat";
                    broadcast_message(clients, new_socket, join_msg);
                    
                    slot_found = true;
                    break;
                }
            }

            if (!slot_found) {
                std::cerr << "Max clients reached, rejecting new connection" << std::endl;
                closesocket(new_socket);
            }
        }

        for (auto& client : clients) {
            if (client.socket != INVALID_SOCKET && FD_ISSET(client.socket, &readfds)) {
                // First read the message length (4 bytes)
                uint32_t msg_length;
                int bytes_received = recv(client.socket, (char*)&msg_length, sizeof(msg_length), 0);
                
                if (bytes_received <= 0) {
                    // Client disconnected
                    std::cout << "Client " << client.username << " " << client.ip_port 
                              << " disconnected at " << get_timestamp() << std::endl;
                    
                    // Notify all clients about user leaving
                    std::string leave_msg = "Client " + client.username + " left the chat";
                    broadcast_message(clients, client.socket, leave_msg);
                    
                    closesocket(client.socket);
                    client.socket = INVALID_SOCKET;
                    continue;
                }

                // Convert from little-endian to host byte order (not needed if same)
                // msg_length = ntohl(msg_length); // Not needed as per requirement #9

                // Then read the actual message
                std::vector<char> buffer(msg_length + 1);
                bytes_received = recv(client.socket, buffer.data(), msg_length, 0);
                
                if (bytes_received <= 0) {
                    // Error receiving message
                    std::cerr << "Error receiving message from " << client.username << std::endl;
                    continue;
                }

                buffer[bytes_received] = '\0';
                std::string message(buffer.data());
                
                // Parse timestamp and actual message (assuming format: "timestamp:message")
                size_t colon_pos = message.find(':');
                if (colon_pos == std::string::npos) {
                    std::cerr << "Invalid message format from " << client.username << std::endl;
                    continue;
                }
                
                std::string timestamp = message.substr(0, colon_pos);
                std::string actual_message = message.substr(colon_pos + 1);
                
                // Display on server
                std::cout << "Client " << client.username << " " << client.ip_port 
                          << " " << timestamp << ": " << actual_message << std::endl;
                
                // Broadcast to all clients (with different format)
                std::string broadcast_msg = "Client " + client.username + " " + timestamp + ": " + actual_message;
                broadcast_message(clients, client.socket, broadcast_msg);
            }
        }
    }

    // Cleanup
    for (auto& client : clients) {
        if (client.socket != INVALID_SOCKET) {
            closesocket(client.socket);
        }
    }
    closesocket(server_socket);
    WSACleanup();

    return 0;
}