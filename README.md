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

## 🍻快速入门

目前此库代码已在以下环境完全通过测试，意味着此库完全支持以下环境编译运行。

 - MSVC (Visual Studio 2019-2022)
 - MinGW (GCC on Windows)
 - Linux GCC/Clang
 - Clang (Termux/Android)

### 先决条件

```
注意！如果你使用的是 Windows 系统，那么应该注意 CHCP 应始终是 65001 。  
尤其是在控制台环境下，否则会出现一些令你感到难受却又找不出问题的 BUG 。
```

 -  最低支持 **C++17** 的编译器（GCC 7+、Clang 5+、MSVC 2017+）。
 -  ~~CMake 3.15+（用于构建）~~，暂时还用不到 **CMake** 。
 -  Python 3.6+。不要担心，Python只是用来更方便的构建脚本所使用的，没有它你其实也可以构建。
 -  此库使用以下开源库（ `Necessity` 仅代表你可以不依赖于对应库来运行此库的基础功能）：  
    | Name                                                | Necessity | Description |
    | ---                                                 | ---       | ---         |
    | [libfmt](https://fmt.dev/)                          | Yes       | 高性能、安全的 C++ 字符串格式化库 |
    | [libsodium](https://doc.libsodium.org/)             | No        | 现代加密库，提供安全加密、签名、哈希等功能 |
    | [libpng](http://www.libpng.org/pub/png/libpng.html) | No        | 处理 PNG 图片的读写官方库 |
    | [libpq](https://www.postgresql.org/)                | No        | PostgreSQL 数据库的 C 语言客户端接口 |
    | [OpenSSL](https://www.openssl.org/)                 | No        | 实现 SSL/TLS 协议及常用加密算法的库 |
    | [Protobuf](https://protobuf.dev/)                   | No        | Google研发的高效的结构化数据序列化工具 |
    | [Zlib](https://www.zlib.net/)                       | No        | 通用数据压缩/解压库（DEFLATE 算法） |

##### 安装必要依赖

Linux, Termux (Android)
```bash
# 更新本地包列表
apt update && apt upgrade -y
# 安装必要的程序
apt install git cmake make gcc g++ -y # 或clang
# 克隆官方仓库
git clone --single-branch git@github.com:fmtlib/fmt.git
# 进入目录
cd fmt
# 编译并安装
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PREFIX
make -j$(nproc)
make install
```

MSYS2 - MinGW
```bash
pacman -Syu
pacman -S mingw-w64-x86_64-fmt
```

最小化测试代码
```cpp
#include <fmt/core.h>
int main() {
    fmt::print("hello, {}\n", "world");
    return 0;
}
```

```bash
# 编译执行
g++ test.cpp -lfmt -o test && ./test # 或替换为clang
```

### 使用

```bash
git clone --single-branch git@github.com:sngrotesque/wizard_key.git
cd wizard_key/

# 使用测试代码进行构建运行测试
python3 py/exec.py [test_file] [complier paramater]

# 比如
python3 py/exec.py test/test.cc
# 再比如
python3 py/exec.py test/im/user.cc -lssl -lcrypto -lpq
```

对于源文件的包含，请查看[exec.py](py/exec.py#L105)中 **main** 函数的部分。

## 文档
 - API参考：详细文档内容可在 [API.md](doc/API.md) 中找到。
 - 更新日志：跟踪 **wizard_key** 的版本修改可在 [CHANGELOG.md](doc/CHANGELOG.md)。
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
 - 封装于 `libpq` 的 `wuk::db::psql`（不使用 `libpqxx` ）。
 - 现代套接字封装：`wuk::net::Socket`。
 - 用于网络数据包协议的 **Protobuf**。
 - IO多路复用支持：select/poll/epoll/IOCP。

### 二进制操作
 - Base64 / Binascii 实现。
 - 二进制序列与数字互转。
