import socket

def dns_lookup(domain_name, dns_server):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.settimeout(5)
    query = create_dns_query(domain_name)
    sock.sendto(query, (dns_server, 53))
    response, server_address = sock.recvfrom(4096)
    sock.close()
    return response

def create_dns_query(domain_name):
    query = b''
    query += b'\x04\xd2' # generate random ID
    query += b'\x01\x00' # set recursion desired flag
    query += b'\x00\x01' # set number of questions
    query += b'\x00\x00' # set number of answers
    query += b'\x00\x00' # set number of authority records
    query += b'\x00\x00' # set number of additional records
    
    for label in domain_name.split('.'):
        # add domain name to query
        query += len(label).to_bytes(1, byteorder='big')
        query += label.encode()
    
    query += b'\x00' # add null byte to end of domain name
    query += b'\x00\x01' # add question type to query
    query += b'\x00\x01' # add question class to query
    
    return query

response = dns_lookup('www.pixiv.net', '192.168.1.1')
response = response[-4:]

print(f'{response[0]}.{response[1]}.{response[2]}.{response[3]}')
