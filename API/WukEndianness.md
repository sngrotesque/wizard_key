# [core/WukEndianness.hh](../wuk/includes/core/WukEndianness.hh)

描述：这是libwuk库用于检测当前环境是否为小端序的模块。
后续可能考虑由使用者决定当前平台端序。

## 端序宏说明
1. **WUK_NATIVE_LE**  
    这是一个小端序宏，如果定义了此宏，说明当前设备是小端序。
2. **WUK_NATIVE_BE**  
    这是一个大端序宏，如果定义了此宏，说明当前设备是大端序。
3. **Unknown**  
    如果前面两个宏都未定义，那么可能需要用户自己确定当前设备端序。  
    一般情况下是不会出现这种情况的，除非你使用的设备非常特殊（比如嵌入式设备）。

# 函数定义

### 函数
1. **wuk::swap_endian**

```cpp
template <typename T>
inline T swap_endian(
    const T &val
);
```

> 这是一个翻转数字端序的函数，用于替代原先的`wuk::reversal_array`函数。

- 参数

`[in] val`
> 这是一个数字类型的值。

- 返回值

> 成功返回切换端序后的数字；否则如果传入的非数字类型将直接返回其自身。
