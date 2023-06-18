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
 *  如果第一个字符不是其中之一，则假定为 '@' 。
 *  本机字节顺序是大端序还是小端序，具体取决于主机系统。例如，Intel x86、AMD64（x86-64）和
 *  Apple M1都是小端序；IBMz和许多遗留体系结构都是big-endian。使用sys.byteorder检查系统的端序。
 *  本机大小和对齐方式是使用 C 编译器的 sizeof 表达式来确定的。 这总是会与本机字节顺序相绑定。
 *  标准大小仅取决于格式字符；请参阅 格式字符 部分中的表格。
 *  请注意 '@' 和 '=' 之间的区别：两个都使用本机字节顺序，但后者的大小和对齐方式是标准化的。
 *  形式 '!' 代表网络字节顺序总是使用在 IETF RFC 1700 中所定义的大端序。
 *  没有什么方式能指定非本机字节顺序（强制字节对调）；请正确选择使用 '<' 或 '>'。
 *  
 *  注释：
 *      1. 填充只会在连续结构成员之间自动添加。 填充不会添加到已编码结构的开头和末尾。
 *      2. 当使用非本机大小和对齐方式即 '<', '>', '=', and '!' 时不会添加任何填充。
 *      3. 要将结构的末尾对齐到符合特定类型的对齐要求，请以该类型代码加重复计数的零作为格式结束。
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
 *  注释：
 *  1.  “？”转换代码对应于C99定义的_Bool类型。如果此类型不可用，则使用char对其进行模拟。
 *     在标准模式下，它总是由一个字节表示。
 *  2. 当尝试使用任何整数转换码打包一个非整数时，如果该非整数具有 __index__() 方法，
 *     则会在打包之前调用该方法将参数转换为一个整数。
 *     在 3.2 版更改: 增加了针对非整数使用 __index__() 方法的特性。
 *  3. 'n' 和 'N' 转换码仅对本机大小可用（选择为默认或使用 '@' 字节顺序字符）。 对于标准大小，
 *     你可以使用适合你的应用的任何其他整数格式。
 *  4. 对于 'f', 'd' 和 'e' 转换码，打包表示形式将使用 IEEE 754 binary32, binary64 或 binary16 格式
 *     (分别对应于 'f', 'd' 或 'e')，无论平台使用何种浮点格式。
 *  5. 'P' 格式字符仅对本机字节顺序可用（选择为默认或使用 '@' 字节顺序字符）。 字节顺序字符 '=' 选择使用
 *     基于主机系统的小端或大端排序。 struct 模块不会将其解读为本机排序，因此 'P' 格式将不可用。
 *  6. IEEE 754 binary16 "半精度" 类型是在 IEEE 754 标准 的 2008 修订版中引入的。 它包含一个符号位，
 *     5 个指数位和 11 个精度位（明确存储 10 位），可以完全精确地表示
 *     大致范围在 6.1e-05 和 6.5e+04 之间的数字。此类型并不被 C 编译器广泛支持：在一台典型的机器上，
 *     可以使用 unsigned short 进行存储，但不会被用于数学运算。请参阅维基百科页面
 *     half-precision floating-point format了解详情。
 *  7. 打包时，“x”插入一个NUL字节。
 *  8. 'p' 格式字符用于编码“Pascal 字符串”，即存储在由计数指定的 固定长度字节 中的可变长度短字符串。
 *     所存储的第一个字节为字符串长度或 255 中的较小值。 之后是字符串对应的字节。
 *     如果传入 pack() 的字符串过长（超过计数值减 1），则只有字符串前 count-1 个字节会被存储。
 *     如果字符串短于 count-1，则会填充空字节以使得恰好使用了 count 个字节。
 *     请注意对于 unpack()，'p' 格式字符会消耗 count 个字节，但返回的字符串永远不会包含超过 255 个字节。
 *  9. 对于's'格式字符，计数被解释为字节的长度，而不是像其他格式字符那样的重复计数；例如，“10s”表示
 *     单个10字节字符串映射到单个Python字节字符串或从单个Pythonbyte字符串映射，而“10c”表示10个单独的
 *     单字节字符元素（例如，cccccccc）映射到10个不同的Python字节对象或从10个不同Python字节对象映射。
 *    （参见例子 以具体证明差异。）如果没有给出计数，则默认为1。对于打包，字符串会被适当地截断或填充
 *     空字节以使其适合。对于解包，生成的字节对象总是具有指定数量的字节。作为特殊情况，“0s”表示单个
 *     空字符串（而“0c”表示0个字符）。
 *  
 *  格式字符之前可以带有整数重复计数。 例如，格式字符串 '4h' 的含义与 'hhhh' 完全相同。
 *  
 *  格式之间的空白字符会被忽略；但是计数及其格式字符中不可有空白字符。
 *  
 *  当使用某一种整数格式 ('b', 'B', 'h', 'H', 'i', 'I', 'l', 'L', 'q', 'Q') 打包值 x 时，
 *  如果 x 在该格式的有效范围之外则将引发 struct.error。
 *  
 *  在 3.1 版更改: 在之前版本中，某些整数格式包装了超范围的值并会引发 DeprecationWarning
 *  而不是 struct.error。
 *  
 *  对于 '?' 格式字符，返回值为 True 或 False。 在打包时将会使用参数对象的逻辑值。
 *  以本机或标准 bool 类型表示的 0 或 1 将被打包，任何非零值在解包时将为 True。
*/
#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_STRUCT
#define WMKC_STRUCT

/**
 * @brief 此函数用于判断本机端序
 * @authors SN-Grotesque
 * 
 * 此函数用于判断本机是大端序还是小端序。
 * 
 * @note 无
 * @param 无
 * @return 返回值为一个布尔值，为True时，本机为小端序。否则为大端序。
*/
WMKC_PUBLIC(wmkcBool) wmkcStruct_PlatformEnd WMKC_OPEN_API
WMKC_OF((wmkcVoid));

/**
 * @brief 此函数用于将数据“打包”
 * @authors SN-Grotesque
 * 
 * 此函数用于将数据“打包”
 * 
 * @note 如果你感到一头雾水，那么请参考Python的struct.pack。
 * @param format 此参数为格式符（必须包含端序符和格式符）。
 *               在端序中，只支持【"\@"，">"，"<"】这三种。
 *               "\@"为按本机原本的端序，也就是保持不变。
 *               ">"为按大端序处理数据（如果本机是大端序，那么等同于"\@"）。
 *               "<"为按小端序处理数据（如果本机是小端序，那么等同于"\@"）。
 *               "H"为使用2字节长度的格式进行处理（uint16_t）。
 *               "I"为使用4字节长度的格式进行处理（uint32_t）。
 *               "Q"为使用8字节长度的格式进行处理（uint64_t）。
 * @param dst 这是一个指针，指向结果的地址（结果可以只是8字节长度）。
 * @param src 这是一个数字，它目前只能是整数。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcStruct_pack WMKC_OPEN_API
WMKC_OF((wmkcCSTR format, wmkcByte *dst, wmkcSize src));

/**
 * @brief 此函数用于将数据“解包”
 * @authors SN-Grotesque
 * 
 * 此函数用于将数据“解包”
 * 
 * @note 如果你感到一头雾水，那么请参考Python的struct.unpack。
 * @param format 此参数为格式符（必须包含端序符和格式符）。
 *               在端序中，只支持【"\@"，">"，"<"】这三种。
 *               "\@"为按本机原本的端序，也就是保持不变。
 *               ">"为按大端序处理数据（如果本机是大端序，那么等同于"\@"）。
 *               "<"为按小端序处理数据（如果本机是小端序，那么等同于"\@"）。
 *               "H"为使用2字节长度的格式进行处理（uint16_t）。
 *               "I"为使用4字节长度的格式进行处理（uint32_t）。
 *               "Q"为使用8字节长度的格式进行处理（uint64_t）。
 * @param dst 这是一个指针，指向结果的地址。
 * @param src 这是一个指针，指向二进制缓冲区的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcStruct_unpack WMKC_OPEN_API
WMKC_OF((wmkcCSTR format, wmkcVoid *dst, wmkcByte *src));

#endif /* WMKC_STRUCT */
#endif /* WMKC_SUPPORT */
