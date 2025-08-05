# 🔐 The Wizard's Universal Key

---

## 🧪 v0.9 更新内容

0. 设计了 **OP4 (four fundamental operations)** 加密算法，以用作未来的密码算法（替代 **SSC** 和 **FEA** 这两个不安全/性能不足的加密算法）。
   1. 它支持高并行化，可使用SIMD高度优化性能（摆脱 SSC / FEA 的自引用依赖导致的性能问题）。
   2. 在未来（预计`v1.0`版本）将会彻底让此算法在保证安全性的情况下实现彻底的并行化以供 [SIMD](https://en.wikipedia.org/wiki/Single_instruction,_multiple_data) 加速。

1. 重新以现代化的方式优化了整体代码并移除了部分“远古”代码。

2. 重新以现代化的标准封装了Socket类（用于未来IM的开发以及彻底替代原先`v0.8`版本时期的屎山代码）；  
   采用 [Protobuf](https://protobuf.dev/) 实现了网络包模块；

3. 以通用方式封装了 [OpenSSL](https://www.openssl.org/) 库提供的哈希算法工具函数，用于更方便的调用哈希算法。  
   当前支持以下哈希算法：  
   | Algorithm | subdivision | Support. | Recommended |
   | --------- | ----------- | -------- | ----------- |
   | MD5       | MD5         | Yes      | No          |
   | SHA-1     | SHA-1       | Yes      | No          |
   | SHA-2     | SHA-224     | Yes      | Yes         |
   |           | SHA-256     | Yes      | Yes         |
   |           | SHA-384     | Yes      | Yes         |
   |           | SHA-512     | Yes      | Yes         |
   | SHA-3     | SHA3-224    | Yes      | Yes         |
   |           | SHA3-256    | Yes      | Yes         |
   |           | SHA3-384    | Yes      | Yes         |
   |           | SHA3-512    | Yes      | Yes         |

4. 对于加密方式，未来网络传输时将采用 **OpenSSL** 库所提供的公钥密码/密钥交换算法，  
   对称加密算法将采用 **OP4** 或 **ChaCha20-Poly1305 (Base on Libsodium, Not OpenSSL)** 或 **AES-256-GCM** 。  
   目前将 **ChaCha20-Poly1305 (RFC 8439)** 作为主要加密算法。

5. 在未来的版本将完全支持 **wuk::net::WukSocket** 库的IO多路复用功能；  
   将利用包括但不限于 **select / poll / epoll / IOCP** 等技术实现。
