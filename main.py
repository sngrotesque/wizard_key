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

def server():
    sockfd = socket.socket()
    sockfd.bind(('0.0.0.0', 49281))
    sockfd.listen(1)
    sockfd.close()

threading.Thread(target=server).start()

client_socket = socket.socket()
client_socket.settimeout(1)
try:
    client_socket.connect(('localhost', 49281))
except ConnectionResetError:
    print("The connection has been reset.")
