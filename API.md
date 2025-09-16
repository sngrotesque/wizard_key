# The Wizard's Universal Key API
> 用于安全、可扩展、跨平台开发的模块化C++框架。

## 目录
### 🧱 项目概览
 - [关于此项目](#关于)
 - [命名规范](#命名规范)

### ⚙️ 核心模块 (Core)
 - [WukConfig.hh](API/WukConfig.md)
 - [WukEndianness.hh](API/WukEndianness.md)
 - [WukException.hh](API/WukException.md)

### 🔐 加密模块 Crypto（待补充）
 - `WukChaCha20.hh`：ChaCha20 支持。
 - `WukCommon.hh`：通用加密库支持。
 - `WukHash.hh`：哈希工具封装。
 - `WukOP4.hh`：OP4 加密算法。

### 💌 IM模块（待补充）
 - `WukIM.hh`：即时通讯软件框架。
 - `WukPsql.hh`：`libpq`库的C++封装。FUCK LIBPQXX。

### 🌐 网络模块 Net（待补充）
 - `WukError.hh`：网络库错误处理。
 - `WukNetwork.hh`：网络库通用头文件。
 - `WukPacket.hh`：网络包协议。
 - `WukSocket.hh`：Socket 封装。

### 🔧 工具模块 Utils（待补充）
 - `bytes.hh`：二进制与十六进制串互转的底层支持。
 - `color.hh`：终端颜色宏。

### 🧩 普通模块（待补充）
 - `WukBase64.hh`：Base64 编码。
 - `WukBinascii.hh`：Hex 编码。
 - `WukBuffer.hh`：网络缓冲区。
 - `WukHexdump.hh`：Hexdump 工具。
 - `WukMemory.hh`：内存管理相关。
 - `WukMisc.hh`：杂项工具，比如打印数据。
 - `WukPadding.hh`：填充算法，后续考虑移除（因加密算法使用流密码算法）。
 - `WukRandom.hh`：随机数，提供伪随机数和密码学安全随机数。
 - `WukSIMD.hh`：SIMD指令函数和类型的简易封装（不考虑使用如`xsimd`库）。
 - `WukTime.hh`：时间相关。

---

## 关于

💥此库最低支持C++17标准💥

每个模块都包含在 **wuk** 命名空间内，对于子项（如 **Crypto** ）会被包含在它自身的命名空间内（比如`wuk::crypto`）。

## 命名规范

对于每个库文件的命名，应遵循`Wuk[xxx]`这样的命名方式，比如我有一个`Duck`功能，那么头文件应该是`WukDuck.hh`，源文件应该是`WukDuck.cc`。  
这样做的目的是为了将来在引用的时候不至于导致编译器匹配了错误的头文件和源文件。

不管是库模块文件的命名还是类的命名，此处都均使用大驼峰命名法。  
对于变量和函数的命名，此处均使用下划线命名法（又叫蛇形命名法），但不拒绝小驼峰命名法。
