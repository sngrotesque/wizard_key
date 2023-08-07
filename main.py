from snpy import *
import matplotlib.pyplot as plt
from hexdump import hexdump
from PIL import Image
import numpy as np
import threading
import requests
import binascii
import base64
import random
import struct
import socket
import qrcode
import json
import time
import zlib
import gzip
import rsa
import ssl
import os
import re

sockfd = socket.socket()
sockfd.settimeout(5)
sockfd.bind(('0.0.0.0', 49281))
sockfd.listen(1)
print(f'等待被连接...')
cSockfd, addr = sockfd.accept()

print(f'开始接收数据...')
tmp = res = b''
try:
    while 1:
        tmp = cSockfd.recv(4096)
        if not tmp:
            break
        res += tmp
except:
    pass

print(f'content: {res}')

cSockfd.shutdown(2)
cSockfd.close()
sockfd.close()
