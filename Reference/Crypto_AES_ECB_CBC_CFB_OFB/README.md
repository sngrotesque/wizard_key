> git@github.com:4thrun/aes.git

# AES

## Tip

Initial vector(AES\_iv\.txt) and key(AES\_key\.txt) should be placed in the folder "file" in the same directory with the \.exe\.

## Commands

```
e2aes -p ./file/AES_plain.txt -k ./file/AES_key.txt -m ECB -v ./file/AES_iv.txt -c ./file/AES_cipher_ECB.txt
e2aes -p ./file/AES_plain.txt -k ./file/AES_key.txt -m CBC -v ./file/AES_iv.txt -c ./file/AES_cipher_CBC.txt
e2aes -p ./file/AES_plain.txt -k ./file/AES_key.txt -m CFB -v ./file/AES_iv.txt -c ./file/AES_cipher_CFB.txt
e2aes -p ./file/AES_plain.txt -k ./file/AES_key.txt -m OFB -v ./file/AES_iv.txt -c ./file/AES_cipher_OFB.txt
```

