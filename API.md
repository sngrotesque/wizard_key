# The Wizard's Universal Key API
> 用于安全、可扩展、跨平台开发的模块化C++框架。

### 💥此库最低支持C++17标准💥

每个模块都包含在 **wuk** 命名空间内，对于子项（如 **Crypto** ）会被包含在它自身的命名空间内（比如`wuk::crypto`）。

目前还保留了如`wByte` / `wSize` / `wU32` 在内的旧类型声明，请不要在库中继续使用了，因为这些旧类型会在未来的版本中被移除。  
请使用 `wuk::byte` / `wuk::ulong` / `wuk::u32` 等新类型替代旧类型。

对于每个库文件的命名，应遵循`Wuk[xxx]`这样的命名方式，比如我有一个`Duck`功能，那么头文件应该是`WukDuck.hh`，源文件应该是`WukDuck.cc`。  
这样做的目的是为了将来在引用的时候不至于导致编译器匹配了错误的头文件和源文件。

不管是库模块文件的命名还是类的命名，此处都均使用大驼峰命名法。  
对于变量和函数的命名，此处均使用下划线命名法（又叫蛇形命名法），但不拒绝小驼峰命名法。

# Core （核心头文件）

## [core/WukConfig.hh](wuk/includes/core/WukConfig.hh)

描述：这是整个libwuk库的核心，包含了类型定义，编译器宏判断，操作系统宏判断，C++标准判断，以及符号接口宏。

### C++标准宏说明
1. **WUK_STD_CPP**  
    此宏作为统一接口（因Visual Studio环境的__cplusplus宏需特别配置才生效），此宏避免了冲突的风险。

```cpp
// 使用方式和`__cplusplus`或`_MSVC_LANG`无差别。
#if WUK_STD_CPP >= 201703L
// ...
#endif
```

### 编译器宏说明
1. **WUK_COMPILER_MSVC**  
    定义了此宏时，代表当前环境为 MSVC 或 Visual Studio。
2. **WUK_COMPILER_MINGW**  
    定义了此宏时，代表当前环境处于MingW32/64中。
3. **WUK_COMPILER_GCC**  
    定义了此宏时，代表当前环境处于GNUC中，比如Linux的GCC环境。
4. **WUK_COMPILER_CLANG**  
    定义了此宏时，代表当前环境为Clang。此库不一定完全支持Clang环境，请自行测试。

### 操作系统宏说明
1. **WUK_PLATFORM_WINOS**  
    定义了此宏时，代表当前操作系统是Windows系统。
2. **WUK_PLATFORM_LINUX**  
    定义了此宏时，代表当前操作系统是Linux系统。
3. **WUK_PLATFORM_ANDROID**  
    定义了此宏时，代表当前操作系统是Android系统（如Termux用户），但请注意由于Termux的GCC实际上是使用的Clang，此库不一定完全支持。
4. **WUK_PLATFORM_MACOS**  
    定义了此宏时，代表当前操作系统是Mac OS系统，此库目前不支持此系统。

### 符号接口宏
1. **LIBWUK_API**  
    在编写类或函数时需加上此宏，以用作编译为库时的符号定义表。用于控制符号导出，确保在 Windows 下 DLL 接口可见，在 Linux 下保持默认行为。

### 类型定义
1. **wuk::byte**   - 这是一个`uint8_t`类型。
2. **wuk::u8**     - 这是一个`uint8_t`类型。
3. **wuk::u16**    - 这是一个`uint16_t`类型。
4. **wuk::u32**    - 这是一个`uint32_t`类型。
5. **wuk::u64**    - 这是一个`uint64_t`类型。
6. **wuk::i8**     - 这是一个`int8_t`类型（注意`int8_t`不等于`char`）。
7. **wuk::i16**    - 这是一个`int16_t`类型。
8. **wuk::i32**    - 这是一个`int32_t`类型。
9. **wuk::i64**    - 这是一个`int64_t`类型。
10. **wuk::ulong** - 这是一个`size_t`类型。
11. **wuk::ilong** - 这是一个`ssize_t`类型。
12. **wuk::f32**   - 这是一个`float`类型，此为了简化写法和统一类型。
13. **wuk::f64**   - 这是一个`double`类型，此为了简化写法和统一类型。

### 函数定义

#### **wuk::min**
> 用于比较两个值的大小。

 - 声明

```cpp
template <typename T>
inline const T &wuk::min(
    const T &x,
    const T &y
);
```

- 参数

`[in] x`  
任何一个支持比较的类型

`[in] y`  
任何一个支持比较的类型

 - 返回值

返回最小的那个值。

#### **wuk::max**
> 用于比较两个值的大小。

 - 声明

```cpp
template <typename T>
inline const T &wuk::max(
    const T &x,
    const T &y
);
```

- 参数

`[in] x`  
任何一个支持比较的类型

`[in] y`  
任何一个支持比较的类型

 - 返回值

返回最大的那个值。

## [core/WukEndianness.hh](wuk/includes/core/WukEndianness.hh)

描述：这是libwuk库用于检测当前环境是否为小端序的模块。
后续可能考虑由使用者决定当前平台端序。

### 宏说明
1. **WUK_NATIVE_LE**  
    这是一个小端序宏，如果定义了此宏，说明当前设备是小端序。
2. **WUK_NATIVE_BE**  
    这是一个大端序宏，如果定义了此宏，说明当前设备是大端序。

### 函数定义

#### **wuk::reversal_array**
> 这是一个翻转序列的函数。后续考虑移除。

 - 声明

```cpp
template <typename T>
void wuk::reversal_array(
    T *buffer,
    wuk::ulong length
);
```

 - 参数

`[in, out] buffer`  
这是一个含多元素的序列；如`std::vector`，`T []`等。

`[in] length`  
此序列的长度。

 - 返回值

无。

## [core/WukException.hh](../wuk/includes/core/WukException.hh)

描述：这是一个异常处理相关的头文件，覆盖整个库的异常类型。
