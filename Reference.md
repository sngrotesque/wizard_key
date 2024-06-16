# Reference

This document is about where I am looking for references. Basically, these libraries are designed based on official documents.

----

### [wmkc::Exception](includes/config/exception.hpp)
### [wmkc::net::Exception](includes/network/exception.hpp)

##### 1. Linux
 - [errno](https://man7.org/linux/man-pages/man3/errno.3.html)
##### 2. Windows
 - [GetLastError](https://learn.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror)
 - [WSAGetLastError](https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsagetlasterror)

---

### [wmkc::Binascii](includes/binascii.hpp)
> The reason for creating this class is simply because I think Binascii in Python is very useful.  
> In terms of performance, I have optimized such methods to the extreme, and in terms of functionality, they are very convenient and easy to use.

 - Inspired by [Python's Binascii module](https://docs.python.org/zh-cn/3/library/binascii.html)

---

### [wmkc::Base64](includes/base64.hpp)

 - [Base64 wiki](https://en.wikipedia.org/wiki/Base64)
 - [Base64 for Python](https://docs.python.org/3/library/base64.html)

---

### [wmkc::padding](includes/padding.hpp)

 - [PKCS7](https://en.wikipedia.org/wiki/PKCS_7)
 - [PKCS padding](https://www.ibm.com/docs/en/zos/3.1.0?topic=rules-pkcs-padding-method)
 - [PKCS padding Wiki](https://en.wikipedia.org/wiki/Padding_(cryptography))
 - [why is padding used in cbc mode](https://crypto.stackexchange.com/questions/48628/why-is-padding-used-in-cbc-mode)

---

### [wmkc::Struct](includes/struct.hpp)
> The reason for creating this class is because I think the Struct module in Python is very useful and can be conveniently used for network data transmission, so I implemented it myself.
 - Inspired by [Python's struct module](https://docs.python.org/zh-cn/3/library/struct.html)

---

### [wmkc::crypto::FEA](includes/crypto/fea.hpp)
> This is a block symmetric encryption algorithm implemented by myself, which has been greatly influenced by the [AES encryption algorithm](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard).

> In order to implement this encryption algorithm, I have referred to a lot of materials, and the following only shows a very small portion of the main reference materials.

 - [Fast encryption algorithm](includes/crypto/fea.hpp)

 - [AES encryption algorithm](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard)
 - [Block cipher mode](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation)
 - [Symmetric key algorithm](https://en.wikipedia.org/wiki/Symmetric-key_algorithm)
 - [Encryption](https://en.wikipedia.org/wiki/Encryption)
 - [Cryptography](https://en.wikipedia.org/wiki/Cryptography)
 - [分组密码的5种工作模式](https://www.bilibili.com/video/BV1U8411f74f/)
 - [SM4 Cipher](https://en.wikipedia.org/wiki/SM4_(cipher))
 - [TLS](https://en.wikipedia.org/wiki/Transport_Layer_Security)
 - [S-Box](https://en.wikipedia.org/wiki/S-box)

---

### [wmkc::net::Socket](includes/network/socket.hpp)

##### 1. Linux
 - [socket](https://man7.org/linux/man-pages/man2/socket.2.html)
 - [connect](https://www.man7.org/linux/man-pages/man2/connect.2.html)
 - [bind](https://man7.org/linux/man-pages/man2/bind.2.html)
 - [send, sendto](https://man7.org/linux/man-pages/man2/sendto.2.html)
 - [recv](https://www.man7.org/linux/man-pages/man2/recv.2.html)
 - [recvfrom](https://man7.org/linux/man-pages/man3/recvfrom.3p.html)
 - [listen](https://man7.org/linux/man-pages/man2/listen.2.html)
 - [accept](https://man7.org/linux/man-pages/man2/accept.2.html)
 - [shutdown](https://man7.org/linux/man-pages/man2/shutdown.2.html)
 - [close](https://man7.org/linux/man-pages/man2/close.2.html)
 - [getsockopt](https://man7.org/linux/man-pages/man2/getsockopt.2.html)
 - [setsockopt](https://pubs.opengroup.org/onlinepubs/009695399/functions/setsockopt.html)
 - [getaddrinfo](https://man7.org/linux/man-pages/man3/getaddrinfo.3.html)
 - [getsockname](https://man7.org/linux/man-pages/man2/getsockname.2.html)
 - [inet_ntop](https://man7.org/linux/man-pages/man3/inet_ntop.3.html)
 - [select](https://man7.org/linux/man-pages/man2/select.2.html)
##### 2. Windows
 - [socket](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket)
 - [connect](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-connect)
 - [bind](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-bind)
 - [send](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-send)
 - [sendto](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-sendto)
 - [recv](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-recv)
 - [recvfrom](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-recvfrom)
 - [listen](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-listen)
 - [accept](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-accept)
 - [shutdown](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-shutdown)
 - [closesocket](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-closesocket)
 - [getsockopt](https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-getsockopt)
 - [setsockopt](https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-setsockopt)
 - [getaddrinfo](https://learn.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo)
 - [getsockname](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-getsockname)
 - [inet_ntop](https://learn.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-inet_ntop)
 - [select](https://learn.microsoft.com/zh-cn/windows/win32/api/winsock2/nf-winsock2-select)