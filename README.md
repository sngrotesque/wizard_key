# 🔐 The Wizard's Universal Key

一个模块化、安全和跨平台的 C++ 框架，用于构建具有加密功能、网络通信和数据处理实用程序的健壮应用程序。

## 概述
巫师的万能密钥（WUK）是一个综合性的 C++ 库，旨在为开发人员提供一套安全应用程序开发的基本工具。  
从加密算法到网络通信和数据结构实用程序，WUK 旨在简化复杂的工作，同时保持高安全标准。

 - 现代 C++：使用 C++17 标准和现代编程实践构建。
 - 模块化设计：组织成逻辑组件，便于集成。
 - 跨平台：支持 Windows、Linux和其他操作系统。
 - 安全重点：实现行业标准和符合行业标准的自研加密算法。
 - 网络：强大的套接字处理和协议支持。

## 快速入门

### 先决条件

 -  最低支持 **C++17** 的编译器（GCC 7+、Clang 5+、MSVC 2017+）。
 -  ~~CMake 3.15+（用于构建）~~，暂时还用不到 **CMake** 。
 -  Python 3.6+。不要担心，Python只是用来更方便的构建脚本所使用的，没有它你其实也可以构建。
 -  此库部分模块依赖以下开源库：  
    | Name      | Website                                                     | Description |
    | ---       | ---                                                         | ---         |
    | Libsodium | [doc.libsodium.org](https://doc.libsodium.org/)             | 流密码库     |
    | Libpng    | [www.libpng.org](http://www.libpng.org/pub/png/libpng.html) | 官方PNG图像库 |
    | Libpq     | [www.postgresql.org](https://www.postgresql.org/)           | PostgreSQL C库 |
    | OpenSSL   | [www.openssl.org](https://www.openssl.org/)                 | OpenSSL库 |
    | Protobuf  | [protobuf.dev](https://protobuf.dev/)                       | Google研发的一款序列化全流程工具 |
    | Zlib      | [www.zlib.net](https://www.zlib.net/)                       | 一个压缩库 |

### 使用

```bash
git clone git@github.com:sngrotesque/wizard_key.git
cd wizard_key/

# 使用测试代码进行构建运行测试
python3 make.py [test_file] [complier paramater]

# 比如
python3 make.py test\test.cc
```

对于源文件的包含，请查看[make.py](make.py#L91)中 **main** 函数的部分。

## 文档
 - API参考：详细文档内容可在 [API.md](API.md) 中找到。
 - 更新日志：跟踪 **wizard_key** 的版本修改可在 [CHANGELOG.md](CHANGELOG.md)。
 - 示例代码：使用此库可参考目录[test](test/mod_test/)。

## 许可证

该项目使用 **Apache 2.0** 许可证，有关详细信息，请参阅 [LICENSE](license) 文件。

## 联系

如有疑问和支持，请在仓库打开一个 **Issues**，感谢理解。

## 主要特点

### 密码学
 - OP4 加密算法。
 - ChaCha20（RFC 8439）实现。
 - SHA-2 和 SHA-3 系列哈希函数。
 - Base64 编码/解码。

### 联网
 - 现代套接字封装：`wuk::net::Socket`。
 - 用于网络数据包协议的 **Protobuf**。
 - IO多路复用支持：select/poll/epoll/IOCP。
