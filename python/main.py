import re
import os
import ssl
import sys
import rsa
import gzip
import zlib
import time
import json
import qrcode
import socket
import struct
import random
import base64
import hashlib
import binascii
import requests
import threading
import numpy as np
from PIL import Image
from hexdump import hexdump
import matplotlib.pyplot as plt
from snpy import *

target_addr = 'passport.bilibili.com'
target_port = 443
target_IPEndPoint = (target_addr, target_port)
userAgent = 'Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/116.0'

sslSockfd = ssl.create_default_context().wrap_socket(socket.socket(), server_hostname=target_addr)
sslSockfd.connect(target_IPEndPoint)
sslSockfd.send((
    f'GET /qrcode/getLoginUrl HTTP/1.1\r\n'
    f'Host: {target_addr}\r\n'
    f'Accept: */*\r\n'
    f'Connection: close\r\n'
    f'User-Agent: {userAgent}\r\n'
    f'\r\n'
).encode())
results = sslSockfd.recv(4096)
sslSockfd.close()

print(results.decode())
