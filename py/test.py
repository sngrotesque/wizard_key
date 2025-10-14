import socket

fd = socket.socket()

fd.connect(('klbq.idreamsky.com', 80))
fd.settimeout(1 / 1e6)
fd.sendall((
    'GET / HTTP/1.1\r\n'
    'Host: klbq.idreamsky.com\r\n'
    'Accept: */*\r\n'
    'User-Agent: Android\r\n\r\n'
).encode())

print(fd.recv(4096))

fd.close()
