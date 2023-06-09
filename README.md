# Wizard Magic Key Cyber
> Wizard Magic Key Cyber by SN-Grotesque

### Author(s):
 - [SN-Grotesque](https://github.com/sngrotesque)
 - [R7](https://github.com/r7z7)

### 特别鸣谢
 - [OpenSSL](https://github.com/openssl/openssl) 本库使用了OpenSSL的哈希与密码库。
 - [libpng](http://libpng.org/pub/png/) 本库使用了libpng库来实现PNG图像的读写。
 - [Python](https://www.python.org/) 本库的绝大多数语法和代码风格均参考过Python的源代码。
 - [cJSON](https://github.com/DaveGamble/cJSON/) 本库采用cJSON库来处理JSON数据。
 - [Zlib](https://github.com/madler/zlib) 本库使用了Zlib库摘要算法库以及参考过源代码的风格。

### 开发文档
 -  关于注释：  
> 此库所有函数统一使用Doxygen风格的注释，且必须包含  
> brief，authors，note，param，return这些字段，并且顺序必须按照此项列出的方式排序。  
> 函数声明与实现的注释必须一样（此条更改为非强制，但函数声明必须有注释）。  
> 如果一个函数没有编写完成就将其推送至仓库，应当在注释中的note字段中说明。  
> 如果是宏函数，那么什么风格的注释随你心意。
 -  关于可读性：
> 所有代码在单行超过90（不含90）个字符时，应当进行换行（非强制）。  
 -  关于返回值：
> 所有应检查参数的函数都应使用wmkcErr_obj对象来返回对应的信息。  
> 并且所有message指针指向的字符串中必须在开头包含对应函数的完整名称。
 -  关于更新：
> 此库的所有更新必须写在更新日志中。
 -  关于第三方库：
> 如果使用了任何第三方库，请在对应的文件中列出，并在此文档的`特别鸣谢`中指出库的名称与对应的链接。
 -  关于C++：
> 此库不应包含任何C++代码。除非非常必要，否则没有商量余地。  
> 如果你要问我对于C++的态度，请参考[Linus Torvalds(Linux之父)](https://medium.com/nerd-for-tech/linus-torvalds-c-is-really-a-terrible-language-2248b839bee3)的态度。
 -  关于定义与实现：
```c
// 所有函数内部都不应使用static将变量声明为静态变量。
// 对上一条的补充：因static变量无法保证线程安全和冲突处理。
// 所有代码文件应在最后一行留出一个空行，方便下次编写。

// 对于函数，应全部使用文件名的形式表示。示例：
// wmkc_duck.h   wmkc_duck.c
WMKC_PUBLIC(wmkcVoid) wmkcDuck_quack WMKC_OPEN_API
WMKC_OF((wmkcVoid));

// 对于对象类型，应使用名称+下划线+obj的方式命名，示例：
typedef struct {
    ...
} wmkcDuck_obj;

// 所有代码头文件的头部应使用预处理指令进行预处理，示例：
#include <wmkc_conf.h>

#if WMKC_SUPPORT  // 此条是为了确保此库可以支持使用者的设备
#ifndef WMKC_DUCK // 判断当前库的宏是否被定义，如果没有
#define WMKC_DUCK // 声明当前库的宏
// 代码
#endif /* WMKC_DUCK */     // 必须使用此格式来表示endif对应哪一条#if语句。
#endif /* WMKC_SUPPORT */

```

### 版本更新日志
> 修改了版本后需要更改的文件  
> includes/wmkc_conf.h  
> upload.py  
> READMKE.md

#### v6.1.0-dev
1.  此版本将重写一些库的实现，包括但不限于  
    wmkcBinascii  
    wmkcBase64  
    wmkcStream  
    wmkcFile

#### v6.0.0-dev
1.  将wmkc_base修改为wmkc_base64，因考虑到  
    不会添加base16（因为此编码可以使用wmkc_binascii库实现）  
    不会添加Base32（因为此编码实在没有必要添加）  
    不会添加Base85（因为此编码（又称Ascii85）目前还没有一个完美的标准）  
    并将Base64的编码与解码库重新设计，去掉了基于wmkc对象的形式。
2.  添加了wmkc_dict库，更好的操作键值对类型的数据。
3.  修改了关于颜色的宏。
4.  修复了wmkc_misc库的BUG。
5.  部署了libpng库，重新构建了zlib库文件。
6.  修复了wmkc_stream库中wmkc_scanf函数的隐藏BUG。
7.  将wmkcStream库中的wmkcStream_scanf函数移动到wmkc_common库并更名为wmkc_scanf。

#### v5.1.0-dev
1.  修改了wmkc_random，wmkc_time库的实现。
2.  修复了wmkc_base库的错误信息不全的问题。
3.  修改了对于系统宏的名称标识。
4.  增加了适当的文档与注释。
5.  增加了wmkc_thread库用于多线程编程。

#### v5.0.0-dev
1.  将库更名为Wizard Magic Key Cyber，原因是与目前某一组织名称重合。
2.  将库函数与一般变量类型与宏函数做了更名。
3.  修改了一些针对不同操作系统的支持。
