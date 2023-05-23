# Shark Coast
> Shark_Coast by SN-Grotesque

```text
这是由SN-Grotesque自主开发的代码框架，用于开发未来的程序。
这个框架提供了SNC对称加密算法，字节码处理，哈希算法处理等功能。

并且有良好的代码注释。
```

### 特别鸣谢
 - [OpenSSL](https://github.com/openssl/openssl)
 - [libpng](http://libpng.org/pub/png/)
 - [Python](https://www.python.org/)
 - [Zlib](https://github.com/madler/zlib)

### v4.8.0
1. 改变了错误代码的类型，将同时返回错误代码与错误文本，更好的纠错。
    为`snConf.h`中定义的错误类型更正了其值所代表的错误含义。
    删除了以往的`snError`类型，新增了`snErr_ctx`类型，为了消除二义性
    将`snErr_ErrNullData`错误代码更名为了`snErr_ErrNULL`，删除
    了`snErr_ErrInvalid`错误类型。
2. 完善了`snNet.h`与`snNet.c`，使其可以兼容多线程编程。
3. 针对错误代码宏的名称的二义性处理，感谢`安洁莉娜克隆中心`的群友：
    [EE0000](https://github.com/ZhaoZuohong)
    [眼镜](https://github.com/Cuthbert-yong)
4. 更改了`snTime.h`与`snTime.c`中`snTime_GetNowTime`函数的实现方式。
5. 修复了`snRand.c`中`snRand_Rand`函数的一个潜在BUG。
6. 增加了一个终端颜色`SN_ALL_COLOR_FLICKER`

