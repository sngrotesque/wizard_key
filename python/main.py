from typing import Callable
import wtools
import socket
import time
import sys
import os

def attack_fcipher(ciphertext_path :str):
    with open(ciphertext_path, 'rb') as f:
        encrypted_content = f.read()

    salt_digest       = encrypted_content[:32]
    salt              = encrypted_content[32:48]
    ciphertext_digest = encrypted_content[48:80]
    ciphertext        = encrypted_content[80:-32]
    ciphertextDigest_width_password_digest = encrypted_content[-32:]

    ciphertext        = os.urandom(256)
    ciphertext_digest = wtools.get_digest(ciphertext)

    with open(ciphertext_path, 'wb') as f:
        f.write(wtools.fcipher.join_bytes(salt_digest, salt, ciphertext_digest, ciphertext,
                ciphertextDigest_width_password_digest))

def fcipher_xcrypt():
    if len(sys.argv) < 5:
        exit(f'usage: python {sys.argv[0]} [Password] [input_path] [output_path] [e]ncrypt/[d]ncrypt')
    
    password = sys.argv[1]
    input_path = sys.argv[2]
    output_path = sys.argv[3]
    xcrypt_mode = sys.argv[4].lower()
    
    if input_path == output_path:
        print(f'input_path == output_path, are you sure?')
        choice = input('[y/N]').lower()
        if (choice == '') or (choice == 'n') or (choice == 'no'):
            exit('exit.')
        elif (choice == 'y') or (choice == 'yes'):
            print('continue.')
        else:
            exit('error input.')
    
    ctx = wtools.fcipher(password.encode())
    
    if xcrypt_mode == 'e':
        ctx.encrypt(input_path, output_path)
    elif xcrypt_mode == 'd':
        ctx.decrypt(input_path, output_path)
    else:
        exit(f'unknown xcrypt_mode.')

def listen(addr :str, port :int = 9971):
    fd = socket.socket()
    fd.bind((addr, port))
    fd.listen(5)
    cfd, caddr = fd.accept()
    
    print(f'Client connected: {caddr[0]}:{caddr[1]}')
    pkt = wtools.packet()
    res = pkt.recv(cfd)
    print(res)
    
    cfd.close()
    fd.close()

def client(addr :str, port :int = 9971):
    fd = socket.socket()
    fd.connect((addr, port))
    
    pkt = wtools.packet()
    pkt.send(fd, b'wtools.packet test!')
    
    fd.close()

if __name__ == '__main__':
    client()
