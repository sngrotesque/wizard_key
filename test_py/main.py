import sys
sys.path.append('E:/Projects/_Library_WMKC')

from snpy import *
import matplotlib.pyplot as plt
from hexdump import hexdump
from PIL import Image
import numpy as np
import threading
import requests
import binascii
import hashlib
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
sockfd.connect(('127.0.0.1', 49281))
print(sockfd.recv(5))
