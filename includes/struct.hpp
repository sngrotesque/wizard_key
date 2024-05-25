#include <config/wmkc.hpp>

/**
 * 对于在使用浮点数的情况下的pack函数出现问题的解决方法：
 *    ！！！你如果是作为字面量传入的话，不要使用直接的整数！！！
 *    ！！！你如果是作为字面量传入的话，不要使用直接的整数！！！
 *    ！！！你如果是作为字面量传入的话，不要使用直接的整数！！！
 *    比如从```pack("df", 0, 1)```改为```pack("df", 0.0, 1.0)```或者```pack("df", 0.0e0, 1.0e0)```。
*/
#if WMKC_SUPPORT
#ifndef WMKC_CPP_STRUCT
#define WMKC_CPP_STRUCT
#include <config/exception.hpp>

#include <any>
#include <vector>
#include <cstdarg>

/**
 *  @brief Python Struct
 *  @link https://docs.python.org/zh-cn/3/library/struct.html
 *  
 *  字节顺序，大小和对齐方法
 *  字符    字节顺序    大小        对齐方式
 *  @       按原字节    按原字节    按原字节
 *  =       按原字节    标准        无
 *  <       小端        标准        无
 *  >       大端        标准        无
 *  !       网络大端    标准        无
 *  
 *  格式字符
 *  | format | C type             | Python type       | Standard size | Notes  |
 *  | ---    | ---                | ---               |               |        |
 *  | x      | pad byte           | no value          |               | (7)    |
 *  | c      | char               | bytes of length 1 | 1             |        |
 *  | b      | signed char        | integer           | 1             | (1)(2) |
 *  | B      | unsigned char      | integer           | 1             | (2)    |
 *  | ?      | _Bool              | bool              | 1             | (1)    |
 *  | h      | short              | integer           | 2             | (2)    |
 *  | H      | unsigned short     | integer           | 2             | (2)    |
 *  | i      | int                | integer           | 4             | (2)    |
 *  | I      | unsigned int       | integer           | 4             | (2)    |
 *  | l      | long               | integer           | 4             | (2)    |
 *  | L      | unsigned long      | integer           | 4             | (2)    |
 *  | q      | long long          | integer           | 8             | (2)    |
 *  | Q      | unsigned long long | integer           | 8             | (2)    |
 *  | n      | ssize_t            | integer           |               | (3)    |
 *  | N      | size_t             | integer           |               | (3)    |
 *  | e      | (6)                | float             | 2             | (4)    |
 *  | f      | float              | float             | 4             | (4)    |
 *  | d      | double             | float             | 8             | (4)    |
 *  | s      | char[]             | bytes             |               | (9)    |
 *  | p      | char[]             | bytes             |               | (8)    |
 *  | P      | void *             | integer           |               | (5)    |
 *  
 *  格式字符之前可以带有整数重复计数。 例如，格式字符串 '4h' 的含义与 'hhhh' 完全相同。
 *  格式之间的空白字符会被忽略；但是计数及其格式字符中不可有空白字符。
*/

namespace wmkc {
    enum class endianness {NO, LE, BE};
    class Struct {
        public:
            Struct() {}

            std::string pack(std::string format, ...);

            std::vector<std::any> unpack(std::string format, std::string buffer);
    };
}

#endif
#endif
