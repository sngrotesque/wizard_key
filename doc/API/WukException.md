# [core/WukException.hh](../wuk/includes/core/WukException.hh)

描述：这是一个异常处理相关的头文件，定义了统一的错误枚举类型`wuk::Error`和
异常类`wuk::Exception`，用于在整个库中进行标准化的错误报告与调试。

# 枚举类型定义

## **wuk::Error**
> 表示库中可能出现的错误类型。

1. **OK**  
    一切正常，无异常。
2. **ERR**  
    未定义的通用异常。
3. **NPTR**  
    空指针调用异常（此类型表示参数传入了空指针）。
4. **NODAT**  
    空数据异常（后续考虑移除）。
5. **MMEORY**  
    内存管理相关异常。
6. **FNOTF**  
    文件未找到。
7. **FTOFO**  
    文件无法打开。

# 类定义

## **wuk::Exception**
> 用于封装错误信息，包括错误代码，函数名，错误详情等。支持通过`what()`方法获取完整错误信息字符串。

### **构造函数**
```cpp
template <typename T>
Exception(
    const T &code,
    const std::string &function,
    const std::string &message
);
```

- 参数

`[in] code`
> 这是错误代码，可为`wuk::Error`或整数。

`[in] function`
> 发生异常的函数名（需要抛出异常的函数的函数名）。

`[in] message`
> 具体错误描述信息。

### **成员函数**

1. **wuk::Exception::get_err_code**
```cpp
inline const wuk::i32 &get_err_code() const noexcept;
```
> 获取当前已指定的错误代码的值。

- 参数
> 无。

- 返回值
> 一个`wuk::i32`类型整数，代表错误代码。

2. **wuk::Exception::get_err_func**
```cpp
inline const std::string &get_err_func() const noexcept;
```
> 获取当前已指定的抛出异常的函数名。

- 参数
> 无。

- 返回值
> 一个`std::string`类型的字符串，表示函数名。

3. **wuk::Exception::get_err_msg**
```cpp
inline const std::string &get_err_msg() const noexcept;
```
> 获取当前已指定的错误信息字符串。

- 参数
> 无。

- 返回值
> 一个`std::string`类型的字符串，表示错误信息。

4. **wuk::Exception::get_err_func**
```cpp
inline const std::string &what() const noexcept;
```
> 获取当前完整的错误信息描述字符串。

- 参数
> 无。

- 返回值
> 一个`std::string`类型的字符串，表示完整的错误信息描述字符串。
