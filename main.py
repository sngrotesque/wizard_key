from snpy import *
import matplotlib.pyplot as plt
from hexdump import hexdump
from PIL import Image
import numpy as np
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

HOSTNAME = 'www.bilibili.com'
HOSTPORT = 443
USERAGENT = "Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0"

sockfd = ssl.SSLContext().wrap_socket(socket.socket(), server_hostname=HOSTNAME)
sockfd.connect(('119.84.174.66', HOSTPORT))
sockfd.send((
    "GET / HTTP/1.1\r\n"
    "Host: " + HOSTNAME + "\r\n"
    "User-Agent: "+USERAGENT+"\r\n\r\n"
).encode())

res = sockfd.recv(4096)
sockfd.close()

print(res)
