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

sockfd = socket.socket(AF_INET6, SOCK_STREAM, 0)
sockfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
# sockfd.bind(('2409:8a62:d13:37c0:e873:f653:8514:3164', 12344))
sockfd.connect(('240e:974:1e01:1100::110', 443))

print(sockfd.getsockname())

sockfd.close()


