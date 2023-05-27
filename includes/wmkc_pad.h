#ifndef WMKC_PADDING
#define WMKC_PADDING

#include <wmkc_conf.h>
#include <wmkc_random.h>
#include <wmkc_time.h>

#define wmkcPad_offset(BlockSize, size) (BlockSize - size % BlockSize)
#define wmkcPad_totalSize(BlockSize, size) (wmkcPad_offset(BlockSize, size) + size)
#define wmkcPad_value 0xac

// 填充函数
WMKC_PUBLIC(wmkcVoid) wmkcPad_add WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize *size, wmkc_u16 BlockSize, wmkcBool randomPadding));

// 删除已填充的数据量
WMKC_PUBLIC(wmkcVoid) wmkcPad_remove WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize *size));

// 为密钥填充
WMKC_PUBLIC(wmkcVoid) wmkcPad_keyAdd WMKC_OPEN_API
WMKC_OF((wmkcByte *token, wmkc_u16 BlockSize));

#endif
