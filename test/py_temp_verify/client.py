import socket
import threading
import struct
import time

quit_command = ['exit', 'quit', 'dc']
stop_event = threading.Event()  # 添加一个事件用于通知线程退出

def fd_send(fd: socket.socket, data: bytes) -> None:
    packet_length = struct.pack('>I', len(data))
    fd.send(packet_length)
    fd.sendall(data)

def fd_recv(fd: socket.socket) -> bytes:
    packet = fd.recv(4)
    if not packet:  # 处理连接关闭的情况
        return b''
    packet_length = struct.unpack('>I', packet)[0]
    packet_data = b''
    while packet_length and not stop_event.is_set():  # 检查退出事件
        _data = fd.recv(min(2048, packet_length))
        if not _data:
            break
        packet_data += _data
        packet_length -= len(_data)
    return packet_data

def receive_messages(client_socket):
    """持续接收服务器广播的消息"""
    while not stop_event.is_set():  # 检查退出事件
        try:
            message = fd_recv(client_socket).decode()
            if not message:
                print("\n与服务器的连接已断开")
                break
            print(f"\n{message}\nEnter message: ", end='')
        except Exception as e:
            if not stop_event.is_set():  # 只有非主动退出时才显示错误
                print(f"\n与服务器的连接异常: {e}")
            break

def start_client():
    global stop_event
    stop_event = threading.Event()  # 初始化事件
    
    username = input("请输入您的用户名: ")
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        client.connect(('127.0.0.1', 47777))
        fd_send(client, username.encode())  # 首先发送用户名
    except ConnectionRefusedError:
        print("无法连接到服务器")
        return

    # 启动接收线程，不设置为守护线程
    receive_thread = threading.Thread(target=receive_messages, args=(client,))
    receive_thread.start()

    while True:
        message = input("Enter message: ")
        if message.lower() in quit_command:
            fd_send(client, message.encode())
            stop_event.set()  # 通知接收线程退出
            client.close()    # 关闭套接字
            receive_thread.join()  # 等待接收线程结束
            break
        fd_send(client, message.encode())

if __name__ == '__main__':
    start_client()