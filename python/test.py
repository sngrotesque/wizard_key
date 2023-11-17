import socket

IPEndPoint = ('240e:930:c200:209::35', 80)

sockfd = socket.socket(socket.AF_INET6, socket.SOCK_STREAM, socket.IPPROTO_TCP)
sockfd.connect(IPEndPoint)
print(sockfd)
sockfd.close()
