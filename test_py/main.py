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

RED   = '\x1b[91m'
CYAN  = '\x1b[96m'
RESET = '\x1b[0m'

def unpack(content :bytes):
    return struct.unpack('>I', content)[0]

def PNG_read(png_path :str):
    with open(png_path, 'rb') as f:
        if f.read(8) != b'\x89\x50\x4E\x47\x0D\x0A\x1A\x0A':
            print(f'{RED}[-]{RESET} {CYAN}{png_path}{RESET} is not a PNG image.')
            exit(-1)
        png_size = unpack(f.read(4))
        f.seek(4, os.SEEK_CUR)
        print(f.read(4))

path = 'p:/VisualStudio2022_profile_1560x1560.png'

PNG_read(path)
