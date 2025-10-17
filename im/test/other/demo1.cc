/* 以下代码由Deepseek实现，我只优化了部分代码，不保证完全的正确性 */
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

#ifdef _MSC_VER
#   pragma comment(lib, "ws2_32.lib")
#endif

#define DEFAULT_PORT 48888
#define BUFFER_SIZE 4096
#define TIMEOUT_SECONDS 30

struct ClientInfo {
    SOCKET socket;
    std::string ip;
    uint16_t port;
    std::vector<char> buffer;
    uint32_t expectedSize;
    uint32_t receivedSize;
};

class TCPServer {
public:
    TCPServer() : m_serverSocket(INVALID_SOCKET) {
        FD_ZERO(&m_readfds);
    }

    ~TCPServer() {
        Cleanup();
    }

    bool Initialize() {
        // 初始化Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
            return false;
        }

        // 创建服务器socket
        m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_serverSocket == INVALID_SOCKET) {
            std::cerr << "socket creation failed: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return false;
        }

        // 设置socket选项（地址复用）
        int opt = 1;
        if (setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
            std::cerr << "setsockopt failed: " << WSAGetLastError() << std::endl;
            closesocket(m_serverSocket);
            WSACleanup();
            return false;
        }

        // 绑定地址和端口
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(DEFAULT_PORT);

        if (bind(m_serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
            closesocket(m_serverSocket);
            WSACleanup();
            return false;
        }

        // 开始监听
        if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR) {
            std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
            closesocket(m_serverSocket);
            WSACleanup();
            return false;
        }

        std::cout << "Server started on port " << DEFAULT_PORT << std::endl;
        return true;
    }

    void Run() {
        timeval timeout;
        timeout.tv_sec = TIMEOUT_SECONDS;
        timeout.tv_usec = 0;

        while (true) {
            // 清空并重置读集合
            FD_ZERO(&m_readfds);
            FD_SET(m_serverSocket, &m_readfds);
            SOCKET maxFd = m_serverSocket;

            // 添加所有客户端socket到读集合
            for (const auto& client : m_clients) {
                FD_SET(client.socket, &m_readfds);
                if (client.socket > maxFd) {
                    maxFd = client.socket;
                }
            }

            // 使用select等待活动socket
            int activity = select(0, &m_readfds, nullptr, nullptr, &timeout);
            if (activity == SOCKET_ERROR) {
                std::cerr << "select error: " << WSAGetLastError() << std::endl;
                break;
            }
            else if (activity == 0) {
                std::cout << "Timeout occurred. No activity for " << TIMEOUT_SECONDS << " seconds. Server shutting down." << std::endl;
                break;
            }

            // 检查服务器socket是否有新连接
            if (FD_ISSET(m_serverSocket, &m_readfds)) {
                AcceptNewConnection();
            }

            // 检查客户端socket的I/O活动
            HandleClientMessages();

            // 如果所有客户端断开连接，服务端关闭
            if (m_clients.empty()) {
                std::cout << "All clients disconnected. Server shutting down." << std::endl;
                break;
            }
        }
    }

private:
    void AcceptNewConnection() {
        sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(m_serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
            return;
        }

        // 获取客户端IP和端口
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        uint16_t clientPort = ntohs(clientAddr.sin_port);

        // 添加到客户端列表
        ClientInfo newClient;
        newClient.socket = clientSocket;
        newClient.ip = clientIP;
        newClient.port = clientPort;
        newClient.expectedSize = 0;
        newClient.receivedSize = 0;
        m_clients.push_back(newClient);

        std::cout << "New client connected: " << clientIP << ":" << clientPort << std::endl;
    }

    void HandleClientMessages() {
        for (auto it = m_clients.begin(); it != m_clients.end(); ) {
            if (FD_ISSET(it->socket, &m_readfds)) {
                char tempBuffer[BUFFER_SIZE];
                int bytesReceived = recv(it->socket, tempBuffer, BUFFER_SIZE, 0);

                if (bytesReceived <= 0) {
                    // 客户端断开连接
                    std::cout << "Client disconnected: " << it->ip << ":" << it->port << std::endl;
                    closesocket(it->socket);
                    it = m_clients.erase(it);
                    continue;
                }

                // 将接收到的数据添加到客户端缓冲区
                it->buffer.insert(it->buffer.end(), tempBuffer, tempBuffer + bytesReceived);

                // 处理缓冲区中的数据
                while (true) {
                    // 如果还没有获取到消息长度
                    if (it->expectedSize == 0 && it->buffer.size() >= sizeof(uint32_t)) {
                        // 读取前4个字节作为消息长度
                        uint32_t msgSize;
                        memcpy(&msgSize, it->buffer.data(), sizeof(uint32_t));
                        it->expectedSize = msgSize;
                        
                        // 移除已处理的4个字节
                        it->buffer.erase(it->buffer.begin(), it->buffer.begin() + sizeof(uint32_t));
                    }

                    // 如果已经获取到消息长度，并且缓冲区中有足够的数据
                    if (it->expectedSize > 0 && it->buffer.size() >= it->expectedSize) {
                        // 提取完整消息
                        std::string message(it->buffer.begin(), it->buffer.begin() + it->expectedSize);
                        
                        // 显示消息
                        std::cout << "Client [" << it->ip << ":" << it->port << "]: " << message << std::endl;
                        
                        // 移除已处理的消息数据
                        it->buffer.erase(it->buffer.begin(), it->buffer.begin() + it->expectedSize);
                        it->expectedSize = 0;
                    }
                    else {
                        break; // 没有完整消息，等待更多数据
                    }
                }
            }
            ++it;
        }
    }

    void Cleanup() {
        // 关闭所有客户端socket
        for (auto& client : m_clients) {
            closesocket(client.socket);
        }

        // 关闭服务器socket
        if (m_serverSocket != INVALID_SOCKET) {
            closesocket(m_serverSocket);
        }

        // 清理Winsock
        WSACleanup();
    }

private:
    SOCKET m_serverSocket;
    fd_set m_readfds;
    std::vector<ClientInfo> m_clients;
};

int main() {
    TCPServer server;
    if (!server.Initialize()) {
        return 1;
    }

    server.Run();
    return 0;
}