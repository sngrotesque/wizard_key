#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_OBJECT
#define WMKC_OBJECT
#include <wmkc_error.h>
#include <wmkc_memory.h>

#ifdef __cplusplus
extern "C" {
#endif

// wmkc对象（后续可能还需要添加一些成员）
typedef struct {
    wmkcByte *buf; // 缓冲区数据
    wmkcSize size; // 缓冲区长度
    wmkcSize index; // 下标（用于在调用find，index函数后查找下一个指定子串）
} wmkc_obj;
/**
 * @brief Common methods for Bytes objects in Python.
 * @param capitalize  返回一个新的对象，其中第一个字节被转换为大写，其余字节保持不变。
 * @param casefold    返回一个新的对象，其中所有字节都被转换为小写，这对于忽略大小写的比较很有用。
 * @param center      返回一个新的对象，其中原始字节被居中对齐，并用指定的字节填充两边，使其达到给定的宽度。
 * @param count       返回子字节序列sub在对象中出现的次数，可选地限制在start和end之间的索引范围内。
 * @param expandtabs  返回一个新的对象，其中所有的制表符（b’\t’）都被替换为指定数量的空格（默认为8个）。
 * @param find        返回子字节序列sub在对象中第一次出现的最小索引，如果没有找到则返回-1，可选地限制在start和end之间的索引范围内。
 * @param index       类似于find ()方法，但如果没有找到子字节序列sub，则抛出ValueError异常。
 * @param isalnum     返回一个布尔值，是否只包含字母或数字，并且不为空。
 * @param isalpha     返回一个布尔值，是否只包含字母，并且不为空。
 * @param isascii     返回一个布尔值，是否只包含ASCII字符，并且不为空。
 * @param isdecimal   返回一个布尔值，是否只包含十进制数字，并且不为空。
 * @param isdigit     返回一个布尔值，是否只包含数字，并且不为空。这包括二进制、八进制、十进制和十六进制数字。
 * @param islower     返回一个布尔值，是否只包含小写字母，并且不为空。
 * @param isnumeric   返回一个布尔值，是否只包含数字字符，并且不为空。这包括数字、分数、下标、上标等字符。
 * @param isprintable 返回一个布尔值，是否只包含可打印字符，并且不为空。可打印字符是空格、字母、数字、标点符号等。
 * @param isspace     返回一个布尔值，表示对象是否只包含空白字符，并且不为空。空白字符包括空格、制表符、换行符、回车符等。
 * @param istitle     返回一个布尔值，表示对象是否是标题化的，并且不为空。标题化意味着每个单词都以大写字母开头，其余字母都是小写的。
 * @param isupper     返回一个布尔值，表示对象是否只包含大写字母，并且不为空。
 * @param lower       返回一个新的对象，其中所有字节都被转换为小写。
 * @param lstrip      返回一个新的对象，其中原始字节左边的所有指定字节（默认为空白字符）都被删除。
 * @param replace     返回一个新的对象，其中所有出现的old子字节序列都被替换为new子字节序列。可选参数count只替换前count次出现的old。
 * @param reverse     返回一个新的对象，其中原始字节的排序被全部反转。
 * @param rfind       类似于find ()方法，但从右边开始查找子字节序列sub。
 * @param rindex      类似于index ()方法，但从右边开始查找子字节序列sub。
 * @param rstrip      返回一个新的对象，其中原始字节右边的所有指定字节（默认为空白字符）都被删除。
 * @param strip       返回一个新的对象，其中原始字节两边的所有指定字节（默认为空白字符）都被删除。
 * @param swapcase    返回一个新的对象，其中所有字节都被转换为相反的大小写。即大写字母变为小写，小写字母变为大写。
 * @param title       返回一个新的对象，其中每个单词都被转换为标题化。即每个单词以大写字母开头，其余字母都是小写的。
 * @param upper       返回一个新的对象，其中所有字节都被转换为大写。
 * @param zfill       返回一个新的对象，其中原始字节左边用零填充，使其达到给定的宽度。如果对象以符号（b’+‘或b’-'）开头，则符号会被保留在最左边。
*/

WMKC_PUBLIC(wmkcErr_obj) wmkcObj_new WMKC_OPEN_API
WMKC_OF((wmkc_obj **obj));

WMKC_PUBLIC(wmkcErr_obj) wmkcObj_free WMKC_OPEN_API
WMKC_OF((wmkc_obj **obj));

WMKC_PUBLIC(wmkcErr_obj) wmkcObj_reverse WMKC_OPEN_API
WMKC_OF((wmkc_obj *obj));

WMKC_PUBLIC(wmkcErr_obj) wmkcObj_append WMKC_OPEN_API
WMKC_OF((wmkc_obj *obj, wmkcCSTR content));

#ifdef __cplusplus
}
#endif

#endif /* WMKC_OBJECT */
#endif /* WMKC_SUPPORT */
