from snpy import *
import matplotlib.pyplot as plt
import binascii
import random
import struct
import socket
import time
import zlib
import rsa
import os

content = wmkcFile_fread('p:/Bilibili/18f7a74b6d5d87269b7bb01cc16771a15035378.jpg')
print(wmkcCrypto_sha256(content))

host = '47.243.162.23'
# host = '127.0.0.1'

ctx = wmkcNet(AF_INET, SOCK_STREAM, host = host, port = 49281)
ctx.timeout(30)
ctx.connect()
ctx.writeChunk(content)
ctx.close()



