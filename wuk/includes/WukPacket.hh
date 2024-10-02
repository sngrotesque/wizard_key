#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_STRUCT
#define WUK_CPP_STRUCT
#include <config/WukException.hh>
#include <network/WukSocket.hh>
#include <WukMemory.hh>
#include <WukTime.hh>

#include <zlib.h>

namespace wuk {
    enum class PacketFlag {
        IS_NONE    = 0x0,  // 00000000 什么都没有（一般用于测试）
        IS_FILE    = 0x1,  // 00000001 此包传输的是文件
        IS_DATA    = 0x2,  // 00000010 此包传输的是有效数据
        IS_SEGMENT = 0x4,  // 00000100 此包传输的已分段数据包
        IS_OVER    = 0x8,  // 00001000 此包是此处传输的最后一个包
        IS_XCRTPT  = 0x10, // 00010000 此包已加密
        IS_MSG     = 0x20, // 00100000 此包为纯消息包
        IS_URGENT  = 0x40, // 01000000 此包为紧急包
        IS_NOERR   = 0x80  // 10000000 校验位，必须为1
    };

    class LIBWUK_API Packet {
    private:
        w_ulong session_id; // 会话ID
    public:
        Packet(w_ulong session_id = 0);
        void send(wuk::net::Socket fd, w_byte *data, w_ulong length, w_u32 sequence);
    };
}

#endif
#endif
