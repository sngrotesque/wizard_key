import binascii
import hashlib
import snpy
import random
import base64

def hexStream2rawStream(data :str):
    return binascii.a2b_hex(''.join(data.split()))

def bytesStream2binaryStream(data :bytes):
    return ''.join([f'{bin(x)[2:]:>08s}' for x in data])

# data = '''7c 23 7e bd 2d 6e fb 67 18 29 24 03 e0 3f a1 ca
#         49 74 6e ea ac 37 4e 18 5a 1f 37 1b 13 5b 1a b2
#         6e 8b 62 8a e8 c2 27 0b 7b d0 df fe af c7 2b 74
#         f2 42 91 87 21 22 3a 2e 3c 9c 2b 44 1b 54 51 43
#         83 84 d5 13 c6 56 33 ba 64 15 16 48 cd 9f 3c 91'''
# res = hexStream2rawStream(data)


data = '/WpE44tIyakzF9Q39cOc8orR1QDY1PHmJYc7qTCX7Rta90ZhtD5dRhsGLsF5/bcsSrf8dr/a8raqXjap6RbLdg=='
res = base64.b64decode(data)
print(res)
