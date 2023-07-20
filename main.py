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

ctx = wmkcNet(AF_INET, SOCK_STREAM, host = '127.0.0.1', port = 49281)
ctx.timeout(30)
ctx.connect()
ctx.writeChunk(content)
ctx.close()



