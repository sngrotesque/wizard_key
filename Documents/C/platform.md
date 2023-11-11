# 不同操作系统的宏定义

```c
#define UNIX      _unix  || _unix_  || __unix
#define Linux     _linux || _linux_ || __linux
#define Windows32 _WIN32 || __WIN32
#define Windows64 _WIN64 || __WIN64
#define IOS       __APPLE__
#define Android   __ANDROID__

#define GCC         __GNUC__
#define Visual_Cpp  _MSC_VER
#define Borland_Cpp __BORLANDC__
```
