# The Wizard's Universal Key API
> 用于安全、可扩展、跨平台开发的模块化C++框架。

## 目录
### 🧱 项目概览
 - [关于此项目](#关于)
 - [编码与命名规范](#编码与命名规范)
 - [禁忌（特别写给跨语言程序员）](#禁忌)

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

## 编码与命名规范

0.  虽然我认为一个合格的代码编写人员不会傻到做这种事情，但我还是需要明确说明！  
    任何使用者不管是在使用此库还是标准库的情况下都 **不应该** 自行修改任何库代码！这将导致严重后果并由修改人自行承担全部责任！  
    如果你想修改对应代码（可能是觉得它不合适或性能问题或别的什么原因），那你应该参与开源建设，而不是直接去修改已发布的库代码。

1.  对于每个库文件的命名，应遵循`Wuk[xxx]`这样的命名方式，比如我有一个`Duck`功能，那么头文件应该是`WukDuck.hh`，源文件应该是`WukDuck.cc`。  
    这样做的目的是为了提高可读性和可维护性，并且统一代码风格。  
    每个模块都包含在 **wuk** 命名空间内，对于子项（如 **Crypto** / **Socket** ）会被包含在它自身的命名空间内（比如`wuk::crypto`/`wuk::net`）。

2.  不管是库模块文件的命名还是类的命名，此处都均使用大驼峰命名法。  
    对于变量和函数的命名，此处均使用下划线命名法（又叫蛇形命名法），但不拒绝小驼峰命名法。

3.  对于所有的结构体，请让它维持在 C 中的职责，即只存放数据，不包含任何方法和构造函数。

4.  对于类类型的要求，构造函数尽量不要使用 [**成员初始化列表（member initializer list）**](https://en.cppreference.com/w/cpp/language/initializer_list.html)
    当前对于移动构造函数等场景是可以使用的，不使用它并不是因为它不好，而是要统一编码风格，毕竟有些构造函数只需要初始化数据成员，导致空着一个`{}`会很不协调。  
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

## 禁忌

### 1. Java/C#的垃圾回收依赖
**问题**：C++没有自动垃圾回收，必须显式管理内存或使用智能指针

```cpp
// 反模式：假设存在自动垃圾回收
void createObjects()
{
    for(int i = 0; i < 1000000; ++i) {
        auto obj = new HeavyObject();  // 不释放！
        obj->process();
    }
}
```

### 2. Python的动态类型滥用
**问题**：C++是静态类型语言，应优先使用明确接口和概念约束

```cpp
// 反模式：试图模仿Python的鸭子类型
void process(auto arg) // C++20概念滥用
{
    arg.fly();  // 编译期才检查，可能引发复杂错误
    arg.swim();
}
```

### 3. JavaScript的回调地狱
**问题**：C++应使用 `future`/`promise` 或协程管理异步

```cpp
// 反模式：深层嵌套lambda
fetchData(Data d{
    process(d, Result r{
        save(r, Status s{
            if(s.ok) log("Done");  // 难以维护的控制流
        });
    });
});
```

### 4. Ruby的元编程狂热
**问题**：C++元编程应限于编译期( `constexpr`/`template` )

```cpp
// 反模式：运行时动态修改类
struct Widget {
    void draw() { /*...*/ }
};

auto hack = []{
    auto mptr = &Widget::draw;
    // 尝试修改成员函数指针——未定义行为！
};
```

### 5. Go的错误处理忽略
**问题**：C++应使用 **异常处理** 或 **错误代码** 等方式明确处理错误

```cpp
// 反模式：忽略错误返回值
void loadConfig() {
    auto file = fopen("config.ini", "r");  // 不检查返回值！
    parse(file);  // 可能解引用空指针
}
```

### 6. Rust的所有权直接移植
**问题**：C++应依赖 **RAII** 和 **智能指针** 体系

```cpp
// 反模式：试图手动实现借用检查器
template<typename T>
class RustLikeRef {
    T* ptr;
    ~RustLikeRef() { ptr=nullptr; }  // 假的"生命周期结束"
};  // C++编译器不会真正阻止use-after-free
```

### 7. Perl的隐式上下文
**问题**：C++需要保持强类型系统

```cpp
// 反模式：函数行为依赖调用上下文
auto getData() { 
    if(/* 神秘条件 */) return 42;
    else return "answer";  // 类型系统被破坏
}
```

### 8. Kotlin的空安全误用
**问题**：C++17后应使用 `std::optional` 明确表达可选值

```cpp
// 反模式：用运算符模拟空安全
template<typename T>
struct Nullable {
    T value;
    operator bool() { return !!value; }  // 危险的类型转换
};
```

### 关键原则总结
1. **内存管理**：C++需要显式资源管理（RAII）
2. **类型系统**：保持静态类型安全
3. **错误处理**：避免静默忽略错误
4. **元编程**：限制在编译期操作
5. **范式匹配**：选择符合语言特性的范式（如C++适合值语义而非纯OOP）

每个语言都有其哲学，跨语言编程时应遵循目标语言的最佳实践而非生搬硬套。