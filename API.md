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

## 编码与命名规范

0.  虽然我认为一个合格的代码编写人员不会傻到做这种事情，但我还是需要明确说明！  
    任何使用者不管是在使用此库还是标准库的情况下都 **不应该** 自行修改任何库代码！这将导致严重后果并由修改人自行承担全部责任！  
    如果你想修改对应代码（可能是觉得它不合适或性能问题或别的什么原因），那你应该参与开源建设，而不是直接去修改已发布的库代码。

1.  对于每个库文件的命名，应遵循`Wuk[xxx]`这样的命名方式，比如我有一个`Duck`功能，那么头文件应该是`WukDuck.hh`，源文件应该是`WukDuck.cc`。  
    这样做的目的是为了提高可读性和可维护性，并且统一代码风格。

2.  不管是库模块文件的命名还是类的命名，此处都均使用大驼峰命名法。  
    对于变量和函数的命名，此处均使用下划线命名法（又叫蛇形命名法），但不拒绝小驼峰命名法。

3.  对于所有的结构体，请让它维持在 C 中的职责，即只存放数据，不包含任何方法和构造函数。

4.  对于类类型的要求，构造函数尽量不要使用 **成员初始化列表（member initializer list）**（当前对于移动构造函数等场景是可以使用的），这并不是因为它不好，而是要统一编码风格，并且有些构造函数其实只需要初始化数据成员，而空着一个`{}`很不协调。  
    并且所有包含私有成员的类的数据成员，都应该命名为`m_xxxx`，比如“名字”就是`std::string m_name`，这样是为了更好的区分不同域之间的变量也为了统一风格。  
    对于只有公共成员的类，你想怎么命名都行，只要别让人读不懂。  
    所有在类方法中使用的类数据成员，都应该加上`this->`前缀，这是为了提升可读性和可维护性，不然和域外的变量重名了很不好分辨。  
    不同作用的方法之间最好用一个`public:`关键字隔开，让人可以一眼看出来它们不是一起的，别觉得多余。  
    下面是一个示例。  
    ```cpp
    class MyClass {
    private:
        wuk::u32 m_age;
        std::string m_name;

    public:
        MyClass() = default;
        MyClass(wuk::u32 age, const std::string &name)
        {
            this->m_age = age;
            this->m_name = name;
        }

    public:
        wuk::u32 get_age() const noexcept
        {
            return this->m_age;
        }

        std::string get_name() const noexcept
        {
            return this->m_name;
        }
    };
    ```

