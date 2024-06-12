import wtools
import socket

def client(addr :str, port :int, path :str):
    client_fd = socket.socket()
    client_fd.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 65536)

    with open(path, 'rb') as f:
        file_data = f.read()

    print(f'connected to [{addr}:{port}]')
    client_fd.connect((addr, port))

    pkt = wtools.packet()
    print('send...')
    pkt.send(client_fd, file_data)
    print('send done.')

    print('recv...')
    print(pkt.recv(client_fd))

    client_fd.close()

if __name__ == '__main__':
    client('192.168.0.103', 9981, 'F:/Pitchers/QQ/QQ_Videos/椰羊.mp4')
