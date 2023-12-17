import random
import struct
import socket

class dns:
    def __init__(self, server_hostname :str, dns_server :str):
        self.server_hostname = server_hostname
        self.dns_server = dns_server

        self.fd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.fd.settimeout(5)

    def __del__(self):
        self.fd.close()

    def create_dns_query(self, server_hostname :str, query_id :int = random.randint(0x0000, 0xffff)):
        query = struct.pack('!HHHHHH',
            query_id, # generate random ID
            0x0100, # set recursion desired flag
            0x0001, # set number of questions
            0x0000, # set number of answers
            0x0000, # set number of authority records
            0x0000) # set number of additional records

        for label in server_hostname.split('.'):
            query += (len(label).to_bytes(1, 'big') + label.encode())

        query += struct.pack('!BHH',
            0x00,    # add null byte to end of domain name
            0x0001,  # add question type to query
            0x0001)  # add question class to query

        return query

    def lookup(self):
        query = self.create_dns_query(self.server_hostname)
        self.fd.sendto(query, (self.dns_server, 53))
        response, server_address = self.fd.recvfrom(4096)
        return response

ctx = dns('www.baidu.com', '223.5.5.5')
print(ctx.lookup())


