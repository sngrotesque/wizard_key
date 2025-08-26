# [core/WukConfig.hh](../wuk/includes/core/WukConfig.hh)

描述：这是整个libwuk库的核心，包含了类型定义，编译器宏判断，操作系统宏判断，C++标准判断，以及符号接口宏。

# C++标准宏说明
1. **WUK_STD_CPP**  
    此宏作为统一接口（因[Visual Studio](https://learn.microsoft.com/zh-cn/cpp/preprocessor/predefined-macros?view=msvc-170)环境的`__cplusplus`宏需特别配置才生效），此宏避免了冲突的风险。

```cpp
// 使用方式和`__cplusplus`或`_MSVC_LANG`无差别。
#if WUK_STD_CPP >= 201703L
// ...
#endif
```

# 编译器宏说明
1. **WUK_COMPILER_MSVC**  
    定义了此宏时，代表当前环境为 MSVC 或 Visual Studio。
2. **WUK_COMPILER_MINGW**  
    定义了此宏时，代表当前环境处于MingW32/64中。
3. **WUK_COMPILER_GCC**  
    定义了此宏时，代表当前环境处于GNUC中，比如Linux的GCC环境。
4. **WUK_COMPILER_CLANG**  
    定义了此宏时，代表当前环境为Clang。此库不一定完全支持Clang环境，请自行测试。

# 操作系统宏说明
1. **WUK_PLATFORM_WINOS**  
    定义了此宏时，代表当前操作系统是Windows系统。
2. **WUK_PLATFORM_LINUX**  
    定义了此宏时，代表当前操作系统是Linux系统。
3. **WUK_PLATFORM_ANDROID**  
    定义了此宏时，代表当前操作系统是Android系统（如Termux用户），但请注意由于Termux的GCC实际上是使用的Clang，此库不一定完全支持。
4. **WUK_PLATFORM_MACOS**  
    定义了此宏时，代表当前操作系统是Mac OS系统，此库目前不支持此系统。

# 符号接口宏
1. **LIBWUK_API**  
    在编写类或函数时需加上此宏，以用作编译为库时的符号定义表。用于控制符号导出，确保在 Windows 下 DLL 接口可见，在 Linux 下保持默认行为。

# 类型定义
1.  **wuk::byte**  - 这是一个`uint8_t`类型。
2.  **wuk::u8**    - 这是一个`uint8_t`类型。
3.  **wuk::u16**   - 这是一个`uint16_t`类型。
4.  **wuk::u32**   - 这是一个`uint32_t`类型。
5.  **wuk::u64**   - 这是一个`uint64_t`类型。
6.  **wuk::i8**    - 这是一个`int8_t`类型（注意`int8_t`不等于`char`）。
7.  **wuk::i16**   - 这是一个`int16_t`类型。
8.  **wuk::i32**   - 这是一个`int32_t`类型。
9.  **wuk::i64**   - 这是一个`int64_t`类型。
10. **wuk::ulong** - 这是一个`size_t`类型。
11. **wuk::ilong** - 这是一个`ssize_t`类型。
12. **wuk::f32**   - 这是一个`float`类型，此为了简化写法和统一类型。
13. **wuk::f64**   - 这是一个`double`类型，此为了简化写法和统一类型。

# 函数定义

### 函数
1. **wuk::min**

```cpp
template <typename T>
inline const T &wuk::min(
    const T &x,
    const T &y
);
```
> 用于比较两个值的大小。

- 参数

`[in] x`
> 任何一个支持比较的类型

`[in] y`  
> 任何一个支持比较的类型

- 返回值

> 返回最小的那个值。

2. **wuk::max**

```cpp
template <typename T>
inline const T &wuk::max(
    const T &x,
    const T &y
);
```
> 用于比较两个值的大小。

- 参数

`[in] x`
> 任何一个支持比较的类型

`[in] y`
> 任何一个支持比较的类型

- 返回值

> 返回最大的那个值。
