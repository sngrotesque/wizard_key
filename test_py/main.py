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

def encrypt(p :int, k :list):
    for i in range(len(k)):
        p ^= k[i]
    return p

k = [194, 45, 244, 120, 181, 125, 175, 216, 156, 102, 74, 141, 216, 152, 121, 129]

for x in range(0x02, 0x102, 0x10):
    print(f'{x:02x}, {encrypt(x, k):02x}')

