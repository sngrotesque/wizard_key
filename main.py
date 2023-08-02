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
import re

sockfd = ssl.SSLContext().wrap_socket(socket.socket(), server_hostname='0.0.0.0')
sockfd.bind(('0.0.0.0', 12345))
sockfd.listen(1)
c_sockfd, addr = sockfd.accept()

sockfd.send(b'...')

c_sockfd.close()
sockfd.close()

