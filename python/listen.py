import socket

IPEndPoint = ('0.0.0.0', 9991)

sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
sockfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sockfd.bind(IPEndPoint)
sockfd.listen(1)

print(f'开始监听：{IPEndPoint}，等待被连接...')
cSockfd, addr = sockfd.accept()
print(f'被{addr}连接。')

print(cSockfd.recv(4096))

cSockfd.close()
sockfd.close()
