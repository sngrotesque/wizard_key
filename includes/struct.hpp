/**
 * 在还未完全开发完毕此功能的情况下，这堆代码就是一坨屎，因为
 * 我现在不想过早优化。
 */
#include <config/wuk.hpp>

#if WUK_SUPPORT
#ifndef WUK_CPP_STRUCT
#define WUK_CPP_STRUCT
#include <config/exception.hpp>
#include <memory.hpp>

#include <any>
#include <vector>
#include <cstdarg>

/**
 * 字节顺序：
 * @    按原字节序
 * =    按原字节序
 * <    小端序
 * >    大端序
 * !    大端序
*/

namespace wuk {
    enum class endianness {
        NO,
        LE,
        BE
    };

    /*
    * 下面这部分代码或许可以去掉了，目前保留着只是为了万一之后用上了，等
    * 完全实现这个功能之后看看情况来决定到底要不要留下这段代码。
    */
    enum class formatType {
        FMT_PAD  = 0,  // [x] padding
        FMT_SC   = 1,  // [c] signed char        (1 byte)
        FMT_SB   = 1,  // [b] FMT_SB == FMT_SC   (1 byte)
        FMT_UC   = 2,  // [B] unsigned char      (1 byte)
        FMT_BOOL = 3,  // [?] _Bool: C99         (1 byte)
        FMT_SSH  = 4,  // [h] signed short       (2 bytes)
        FMT_USH  = 5,  // [H] unsigned short     (2 bytes)
        FMT_SI   = 6,  // [i] int                (4 bytes)
        FMT_UI   = 7,  // [I] unsigned int       (4 bytes)
        FMT_SL   = 8,  // [l] long               (4 bytes)
        FMT_UL   = 9,  // [L] unsigned long      (4 bytes)
        FMT_SQ   = 10, // [q] long long          (8 bytes)
        FMT_SN   = 10, // [n] FMT_SN == FMT_SQ   (8 bytes)
        FMT_UQ   = 11, // [Q] unsigned long long (8 bytes)
        FMT_UN   = 11, // [N] FMT_UN == FMT_UQ   (8 bytes)
        FMT_F    = 12, // [f] float              (4 bytes)
        FMT_D    = 13, // [d] double             (8 bytes)

        /**
         * 对于's'格式字符，计数会被解读为字节的长度，而不是像其他格式字符那样的重复计数；
         * 例如，'10s'表示一个与特定的Python字节串互相映射的长度为10的字节数据，
         * 而'10c'则表示个10个与十个不同的Python字节对象互相映射的独立的一字节字符元素(如cccccccccc)。
         * 如果未给出计数，则默认值为1。对于打包操作，字节串会被适当地截断或填充空字节以符合尺寸要求。
         * 对于解包操作，结果字节对象总是会恰好具有指定数量的字节。作为特例，'0s'表示单个空字节串(而'0c'表示0个字符)。
        */
        FMT_S    = 14, // [s] const char *

        /**
         * 'p'格式字符用于编码“Pascal字符串”，即存储在由计数指定的固定长度字节中的可变长度短字符串。
         * 所存储的第一个字节为字符串长度或255中的较小值，之后是字符串对应的字节。
         * 如果传入pack()的字符串过长（超过计数值减1），则只有字符串前count-1个字节会被存储。
         * 如果字符串短于count-1，则会填充空字节以使得恰好使用了count个字节。
         * 请注意对于unpack()，'p'格式字符会消耗count个字节，但返回的字符串永远不会包含超过255个字节。
        */
        FMT_P    = 15, // [p] const char *

        FMT_ERR  = 255, // format error
    };
    class FormatArgs {
    public:
        formatType  type;   // 参数类型
        w_u32       count;  // 参数数量
        std::string result; // 格式串转为二进制序列后的结果
    };
}

namespace wuk {
    class LIBWUK_API Struct {
    private:
        // data member
        bool is_switch_endianness;

    private:
        // function member
        void reverse_array(char *array, w_u32 size);

        template <typename T>
        std::string foramt_number_option(std::vector<T> args);

    public:
        Struct();

        template <typename T>
        std::string format_parser(char format, wSize count, std::vector<T> args);

        std::string pack(std::string format_string, std::vector<std::any> args);
        std::vector<std::any> unpack(std::string format_string, std::string buffer);
    };
}

#endif
#endif
