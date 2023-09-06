#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_PNG
#define WMKC_PNG
#include <wmkc_memory.h>
#include <wmkc_struct.h>
#include <wmkc_stream.h>
#include <wmkc_file.h>

#define WMKC_PNG_HEAD "\x89\x50\x4e\x47\x0d\x0a\x1a\x0a" // PNG图像结构文件头

typedef struct wmkcPNG_chunk {
    wmkcByte size[4];
    wmkcByte type[4];
    wmkcByte *data;
    wmkcByte crc[4];
    wmkc_u32 sizeN;
    struct wmkcPNG_chunk *next;
} wmkcPNG_chunk;

typedef struct {
    wmkc_u32 width;
    wmkc_u32 height;
    wmkcByte bitDepth;
    wmkcByte colorType;
    wmkcByte compressMethod;
    wmkcByte filterMethod;
    wmkcByte interlaceMethod;
    wmkcPNG_chunk *chunk;
} wmkcPNG_obj;

// IHDR块与IEND块默认包含
typedef enum {
    IDAT = (1 << 0),
    tEXt = (1 << 1),
    sRGB = (1 << 2),
    pHYs = (1 << 3),
    iTXt = (1 << 4),
    iCCP = (1 << 5),
    cHRM = (1 << 6),
    gAMA = (1 << 7),
    sBIT = (1 << 8),
    hIST = (1 << 9),
    sPLT = (1 << 10),
    bKGD = (1 << 11),
    tIME = (1 << 12),
    tRNS = (1 << 13),
    zTXt = (1 << 14)
} wmkcPNG_type;

/**
 * @brief 为wmkcPNG对象申请内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcPNG对象指针的地址
 * @return wmkcErr对象code为非0代表出错，需检查。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcPNG_new WMKC_OPEN_API
WMKC_OF((wmkcPNG_obj **obj));

/**
 * @brief 为wmkcPNG对象释放内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcPNG对象指针的地址
 * @return wmkcErr对象code为非0代表出错，需检查。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcPNG_free WMKC_OPEN_API
WMKC_OF((wmkcPNG_obj **obj));

/**
 * @brief 读取PNG图像函数
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcPNG对象的地址
 * @param type 块类型，指定需要匹配的块类型，默认匹配IHDR和IEND块。
 *             如果值为0，那么匹配所有块。
 * @param path 图像路径
 * @return wmkcErr对象code为非0代表出错，需检查。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcPNG_read WMKC_OPEN_API
WMKC_OF((wmkcPNG_obj *obj, wmkcPNG_type type, wmkcCSTR path));

#endif /* WMKC_PNG */
#endif /* WMKC_SUPPORT */
