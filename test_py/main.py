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

ctx = wmkcCrypto(b'0123456789abcdef', b'0123456789abcdef', None, AES.MODE_CFB, None, None, 12)
res = ctx.aes_encrypt(b'1234567890')
print(res)

