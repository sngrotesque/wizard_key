# 生成自己的动静态库

[参考链接：Linux下动态链接库与静态链接库（编写、编译）](https://blog.csdn.net/TABE_/article/details/114897323)

### 注意事项
> `g++`无法使用`c`文件编写的动静态库，`gcc`无法使用`cpp`文件编写的动静态库，函数声明（外部暴露函数）不要带`static`关键字。

- 使用动态库
> 库文件名需为`lib`开头，并且在`-l`时不添加`lib`字段，如：`libtest.so` -> `gcc test.c -ltest -o test`。
```bash
gcc {filename}.c -L{path} -l{library name} -o {ELF or PE}
```

- 查看库内符号链接
```bash
nm -s {filename}
```

### 动态库

- 创建动态库
```bash
gcc {name}.c -fPIC -shared -o lib{name}.so
```

- 简单说明
    -shared 该选项指定生成动态连接库  
    -fPIC 表示编译为位置独立的代码，不用此选项的话编译后的代码是位置相关的，所以动态载入时是通过代码拷贝的方式来满足不同进程的需要，而不能达到真正代码段共享的目的。

### 静态库

- 创建静态库
```bash
gcc -c {filename}.c # 得到 lib.o
ar cr {library filename} {filename}.o
```

- 简单说明
    c 不在必须创建库的时候给出警告
    r 替换归档文件中已有的文件或加入新文件
