### Shark Coast
> Shark_Coast by SN-Grotesque

这是由SN-Grotesque自主开发的代码框架，用于开发未来的程序。
这个框架提供了SNC对称加密算法，字节码处理，哈希算法处理等功能。

并且有良好的代码注释。

# v4.8.0 更新日志
1. 改变了错误代码的类型，将同时返回错误代码与错误文本，更好的纠错。<br>
    删除了以往的snError类型，新增了snErr_ctx类型。<br>
    类型原型可以在includes/snConf.h文件中找到。<br>
    为snConf.h中定义的错误类型更正了其值所代表的错误含义。
2. 完善了snNet库，使其可以兼容多线程编程。
3. 针对错误代码宏的名称的二义性处理，感谢`安洁莉娜克隆中心`的群友：<br>
    [EE0000](https://github.com/ZhaoZuohong)<br>
    [眼镜](https://github.com/Cuthbert-yong)



