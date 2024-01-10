### Big-endian

1. packet seq:    4 Bytes.
2. packet length: 4 Bytes.
3. packet crc32:  4 Bytes (seq + length).
4. packet data:   [packet length] Bytes.
5. packet sha256: 32 Bytes.
6. packet end:    8 Bytes. [53 4e 45 03 4e 04 45 05]




