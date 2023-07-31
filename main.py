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

aes = wmkcCrypto(key = b'1234567890abcdef', iv = b'0123456789abcdef',
    nonce = b'804139ha', segment_size = 32, mode = AES.MODE_CTR)

content = 'u8 *((**(*海嗣)(u8 *, u32))[4])(u32 *, u8 **)'.encode()
content = wmkcCrypto_padding(content)
res = aes.aes_encrypt(content)

print(res)
print(res.hex())
print(base64.b64encode(res).decode())

