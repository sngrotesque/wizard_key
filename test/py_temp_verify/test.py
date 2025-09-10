import socket
import struct

fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
fd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, True)
fd.bind(('0.0.0.0', 48888))
fd.listen(5)

client_fd, addr = fd.accept()
print(client_fd.recv(256))

client_fd.send(b'The message was received correctly without error.')

client_fd.close()
fd.close()
