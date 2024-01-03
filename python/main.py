import hashlib
import snpy

with open('./dns_lookup.py', 'rb') as f:
    print(snpy.wmkcCrypto_hashlib(hashlib.sha256(), data = f.read()))


