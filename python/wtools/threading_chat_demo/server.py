import os
import socket
import threading

print(f'current program pid: {os.getpid()}')

heartBeatPacket = b'&&&&&&&&'

clients = []

def handle_client(client_socket):
    while True:
        try:
            data = client_socket.recv(1024)
            if not data:
                break
            print(f"Received: {data}")
            for c in clients:
                c.send(heartBeatPacket)
        except ConnectionResetError:
            break
    clients.remove(client_socket)
    client_socket.close()

def server(laddr :str = '0.0.0.0', lport :int = 37789):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((laddr, lport))
    server_socket.listen(5)
    print(f"Server listening on port {lport}...")
    
    while True:
        client_sock, addr = server_socket.accept()
        print(f"Accepted connection from {addr}")
        clients.append(client_sock)
        
        client_thread = threading.Thread(target=handle_client, args=(client_sock,))
        client_thread.start()

if __name__ == "__main__":
    server_thread = threading.Thread(target=server)
    server_thread.start()