### Shark Coast
> Shark_Coast by SN-Grotesque

这是由SN-Grotesque自主开发的代码框架，用于开发未来的程序。
这个框架提供了SNC对称加密算法，字节码处理，哈希算法处理等功能。

并且有良好的代码注释。

# v4.8.0 更新日志
1. 改变了错误代码的形式，将同时返回错误代码与错误文本，更好的纠错。
    删除了以往的snError类型，新增了snErr_ctx类型。
    类型原型可以在includes/snConf.h文件中找到。
2. 完善了snNet库，使其可以兼容多线程编程。
