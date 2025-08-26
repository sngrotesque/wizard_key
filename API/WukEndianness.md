# [core/WukEndianness.hh](../wuk/includes/core/WukEndianness.hh)

描述：这是libwuk库用于检测当前环境是否为小端序的模块。
后续可能考虑由使用者决定当前平台端序。

## 端序宏说明
1. **WUK_NATIVE_LE**  
    这是一个小端序宏，如果定义了此宏，说明当前设备是小端序。
2. **WUK_NATIVE_BE**  
    这是一个大端序宏，如果定义了此宏，说明当前设备是大端序。

# 函数定义

### 函数
1. **wuk::reversal_array**

```cpp
template <typename T>
inline void wuk::reversal_array(
    T *buffer,
    wuk::ulong length
);
```

> 这是一个翻转序列的函数。后续考虑移除。

- 参数

`[in, out] buffer`
> 这是一个含多元素的序列；如`std::vector`，`T []`等。

`[in] length`
> 此序列的长度。

- 返回值

> 无。

2. **wuk::swap_endian**

```cpp
template <typename T>
inline T swap_endian(
    const T &val
);
```

> 这是一个翻转数字端序的函数，用于替代`wuk::reversal_array`函数。

- 参数

`[in] val`
> 这是一个数字类型的值。

- 返回值

> 成功返回切换端序后的数字；否则如果传入的非数字类型将直接返回其自身。
