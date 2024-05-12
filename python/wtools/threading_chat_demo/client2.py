# client.py
import socket
import threading

def receive_messages(sock):
    while True:
        try:
            # 接收数据
            data = sock.recv(1024)
            if not data:
                break
            # 如果收到的是心跳包，则不打印
            # if data != b'&&&&&&&&':
            #     print(f"Received: {data}")
            print(f'Received: {data}')
        except ConnectionResetError:
            break

def send_messages(sock):
    while True:
        # 发送数据
        message = input("Enter message: ")
        sock.sendall(message.encode())

def client(host='localhost', port=37789):
    # 连接到服务器
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, port))
    
    # 创建接收和发送消息的线程
    receiver_thread = threading.Thread(target=receive_messages, args=(sock,))
    sender_thread = threading.Thread(target=send_messages, args=(sock,))
    
    receiver_thread.start()
    sender_thread.start()

    receiver_thread.join()
    sender_thread.join()

if __name__ == "__main__":
    client()
