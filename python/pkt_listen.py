import wtools
import socket

def listen(addr :str, port :int, path :str):
    listen_fd = socket.socket()
    listen_fd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    listen_fd.bind((addr, port))
    listen_fd.listen(5)
    
    print(f'waiting client connected...')
    client_fd, client_addr = listen_fd.accept()
    
    print(f'client connected: [{client_addr[0]}:{client_addr[1]}]')
    pkt = wtools.packet()

    print('recv...')
    content = pkt.recv(client_fd)
    file_data = content[3]
    print('recv done.')

    print('send.')
    pkt.send(client_fd, b'done.')

    print(f'save to {path}')
    with open(path, 'wb') as f:
        f.write(file_data)

    client_fd.close()
    listen_fd.close()

if __name__ == '__main__':
    listen('0.0.0.0', 9981, 'test_file')
