from .utils import *
from .snCrypto import snCrypto

class software:
    def __init__(self,
            FILE_ENCRYPTION_TYPE :str,
            FILE_COMPRESSION_TYPE :str,
            FILE_TYPE :str = '00'
        ):
        self.FILE_HEAD_TYPE = a2b(f'534e424547494e{FILE_TYPE}')
        self.FILE_END_TYPE = a2b(f'534e454e44{FILE_TYPE}')
        self.FILE_ENCRYPTION_TYPE = FILE_ENCRYPTION_TYPE
        self.FILE_COMPRESSION_TYPE = FILE_COMPRESSION_TYPE

    def pack(self, path :str):
        f = fileSet(path)
        return f




