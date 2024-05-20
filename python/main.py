import wtools
import os

def attack_fcipher(ciphertext_path :str):
    with open(ciphertext_path, 'rb') as f:
        encrypted_content = f.read()

    salt_digest       = encrypted_content[:32]
    salt              = encrypted_content[32:48]
    ciphertext_digest = encrypted_content[48:80]
    ciphertext        = encrypted_content[80:-32]
    ciphertextDigest_width_password_digest = encrypted_content[-32:]

    ciphertext        = os.urandom(256)
    ciphertext_digest = wtools.get_digest(ciphertext)

    with open(ciphertext_path, 'wb') as f:
        f.write(wtools.fcipher.join_bytes(salt_digest, salt, ciphertext_digest, ciphertext,
                ciphertextDigest_width_password_digest))

def main(path :str, original_file_name :str):
    ctx = wtools.fcipher(b'sngrotesque')
    ctx.encrypt(f'{path}/{original_file_name}', f'{path}/cipher')
    ctx.decrypt(f'{path}/cipher', f'{path}/plain')

main('C:/Users/sn/Desktop/收纳/fcipher', 'original.mp3')

