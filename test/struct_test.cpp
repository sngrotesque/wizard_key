// python make.py test\struct_test.cpp -O3 -Wall -DWUK_EXPORTS
#include <config/wuk.hpp>
#include <config/exception.hpp>

#include <cstdarg>
#include <cmath>

#include <iostream>
#include <vector>
#include <any>

using namespace std;

/**
 * 字节顺序：
 * @    按原字节序
 * =    按原字节序
 * <    小端序
 * >    大端序
 * !    大端序
*/

// 以下缩写未在任何程度映射现实中的机构或任何现有程序缩写
enum class formatType {
    FMT_PAD  = 0x0,    // padding
    FMT_SC   = 0x1,    // signed char        (1 byte)
    FMT_SB   = 0x1,    // FMT_SB == FMT_SC   (1 byte)
    FMT_UC   = 0x2,    // unsigned char      (1 byte)
    FMT_BOOL = 0x4,    // _Bool: C99         (1 byte)
    FMT_SSH  = 0x8,    // signed short       (2 bytes)
    FMT_USH  = 0x10,   // unsigned short     (2 bytes)
    FMT_SI   = 0x20,   // int                (4 bytes)
    FMT_UI   = 0x40,   // unsigned int       (4 bytes)
    FMT_SL   = 0x80,   // long               (4 bytes)
    FMT_UL   = 0x100,  // unsigned long      (4 bytes)
    FMT_SQ   = 0x200,  // long long          (8 bytes)
    FMT_UQ   = 0x400,  // unsigned long long (8 bytes)
    FMT_SN   = 0x800,  // FMT_SN == FMT_SQ   (8 bytes)
    FMT_UN   = 0x1000, // FMT_UN == FMT_UQ   (8 bytes)
    FMT_F    = 0x2000, // float              (4 bytes)
    FMT_D    = 0x4000, // double             (8 bytes)

    /**
     * 对于's'格式字符，计数会被解读为字节的长度，而不是像其他格式字符那样的重复计数；
     * 例如，'10s'表示一个与特定的Python字节串互相映射的长度为10的字节数据，
     * 而'10c'则表示个10个与十个不同的Python字节对象互相映射的独立的一字节字符元素(如cccccccccc)。
     * 如果未给出计数，则默认值为1。对于打包操作，字节串会被适当地截断或填充空字节以符合尺寸要求。
     * 对于解包操作，结果字节对象总是会恰好具有指定数量的字节。作为特例，'0s'表示单个空字节串(而'0c'表示0个字符)。
     */
    FMT_S    = 0x8000, // const char *

    /**
     * 'p'格式字符用于编码“Pascal字符串”，即存储在由计数指定的固定长度字节中的可变长度短字符串。
     * 所存储的第一个字节为字符串长度或255中的较小值，之后是字符串对应的字节。
     * 如果传入pack()的字符串过长（超过计数值减1），则只有字符串前count-1个字节会被存储。
     * 如果字符串短于count-1，则会填充空字节以使得恰好使用了count个字节。
     * 请注意对于unpack()，'p'格式字符会消耗count个字节，但返回的字符串永远不会包含超过255个字节。
     */
    FMT_P    = 0x10000 // const char *
};

class FormatArgs {
public:
    formatType  type;   // 参数类型
    w_u32       count;  // 参数数量
    std::string result; // 格式串转为二进制序列后的结果
};

/**
 * @brief 格式字符串解析函数
 * @authors SN-Grotesque
 * @note 无
 * @param buffer 传入时必须是一个限定了长度的字符串，比如母串是"I3H5BQQQQd"，那么
 *               传入的分别参数必须是"I", "3H", "5B", "Q", "Q", "Q", "Q", "d"。
 * @return 无
 */
FormatArgs format_string_parser(const std::string format_args)
{
    const char *fmt_ptr = format_args.c_str();
    FormatArgs result{};

    // 如果第一个字符不是数字
    if(!isdigit(*fmt_ptr)) {
        result.count = 1;

        switch(*fmt_ptr) {

        }
    }
    // 否则
    else {
        
    }


    // w_long number = strtol(p, nullptr, 10);


    return result;
}

class Struct {
public:
    std::string pack(std::string foramt_string, ...);
};

int main(int argc, char **argv)
{
    const char *buffer = "3I";

    return 0;
}
