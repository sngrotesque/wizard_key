# [core/WukEndianness.hh](../wuk/includes/core/WukEndianness.hh)

描述：这是libwuk库用于检测当前环境是否为小端序的模块。
后续可能考虑由使用者决定当前平台端序。

## 端序宏说明
1. **WUK_NATIVE_LE**  
    这是一个小端序宏，如果定义了此宏，说明当前设备是小端序。
2. **WUK_NATIVE_BE**  
    这是一个大端序宏，如果定义了此宏，说明当前设备是大端序。

## 函数定义WukEndianness

### **wuk::reversal_array**

```cpp
template <typename T>
void wuk::reversal_array(
    T *buffer,
    wuk::ulong length
);
```

> 这是一个翻转序列的函数。后续考虑移除。

 - 参数

`[in, out] buffer`  
这是一个含多元素的序列；如`std::vector`，`T []`等。

`[in] length`  
此序列的长度。

 - 返回值

无。
