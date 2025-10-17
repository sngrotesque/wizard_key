import threading
import socket
import struct
import time

clients = {}  # 格式: {client_socket: (username, addr)}
clients_lock = threading.Lock()
server_active = True  # 控制服务器运行状态

def broadcast(sender_socket, message: str):
    """广播消息给所有客户端（除发送者外）"""
    with clients_lock:
        for client in list(clients.keys()):  # 避免遍历时修改字典
            if client != sender_socket:
                try:
                    fd_send(client, message.encode())
                except:
                    remove_client(client)

def remove_client(client_socket):
    """移除断开连接的客户端"""
    with clients_lock:
        if client_socket in clients:
            username, addr = clients[client_socket]
            print(f"[断开连接] 用户 {username} ({addr}) 已退出")
            del clients[client_socket]
            client_socket.close()
            # 检查是否所有客户端断开
            if not clients and server_active:
                print("所有用户已断开，服务器即将关闭...")
                global_server_socket.close()  # 关闭主监听套接字

def handle_client(client_socket, addr):
    """处理客户端连接"""
    try:
        # 接收用户名
        username = fd_recv(client_socket).decode()
        with clients_lock:
            clients[client_socket] = (username, addr)
        print(f"[新连接] 用户 {username} ({addr}) 加入聊天室")
        broadcast(client_socket, f"系统通知: {username} 进入了聊天室")

        while True:
            data = fd_recv(client_socket).decode()
            if not data or data.lower() in ['exit', 'quit']:
                break
            # 广播格式: [用户名] 消息内容
            broadcast(client_socket, f"[{username}] {data}")
    except ConnectionResetError:
        pass
    finally:
        remove_client(client_socket)

def start_server(host='0.0.0.0', port=47777):
    global global_server_socket
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((host, port))
    server.listen(5)
    global_server_socket = server
    print(f"服务器已启动，监听 {host}:{port}")

    try:
        while server_active:
            client_fd, addr = server.accept()
            threading.Thread(target=handle_client, args=(client_fd, addr)).start()
    except OSError:  # 主套接字被关闭时退出
        pass
    print("服务器已关闭")

# 复用原有的数据包处理函数
def fd_send(fd: socket.socket, data: bytes) -> None:
    packet_length = struct.pack('>I', len(data))
    fd.send(packet_length)
    fd.sendall(data)

def fd_recv(fd: socket.socket) -> bytes:
    packet = fd.recv(4)
    packet_length = struct.unpack('>I', packet)[0]
    packet_data = b''
    while packet_length:
        _data = fd.recv(min(2048, packet_length))
        packet_data += _data
        packet_length -= len(_data)
    return packet_data

if __name__ == '__main__':
    start_server()