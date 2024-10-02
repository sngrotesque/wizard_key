/**
 * 具体包的设计标准请参考`doc/network_packet.md`文件。
 * 
 * @date 2024-10-03
 * @copyright Copyright (c) 2024
 */
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_PACKET
#define WUK_CPP_PACKET
#include <config/WukException.hh>
#include <network/WukSocket.hh>
#include <WukMemory.hh>
#include <WukTime.hh>

#include <zlib.h>

namespace wuk {
    namespace net {
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

        class LIBWUK_API PacketEndianness {
        public:
            void reverse_array(wByte *array, wU32 size);

            template <typename T>
            void write_bytearray(wByte *array, wSize array_size, T val);

            template <typename T>
            T read_bytearray(wByte *array);
        };

        class LIBWUK_API PacketMate {
        public:
            wByte packet_time[8];
            wByte session_id[8];
            wByte sequence[4];
            wByte mate_crc[4];

            PacketMate();

            void write_time(double current_time);
            void write_session_id(w_ulong id);
            void write_sequence(w_u32 seq);
            void write_mate_crc();

            double read_time();
            w_ulong read_session_id();
            w_u32 read_sequence();
            w_u32 read_mate_crc();
        };

        class LIBWUK_API PacketData {
        private:
            w_u32 length_val; // 添加这个变量的目的是想实现那样append方法。

        public:
            wByte *data;
            wByte length[4];
            wByte dataCRC[4];

            PacketData();

            void write_data(wByte *data, wSize data_length);
            void write_crc();

            wByte *read_data();
            w_u32 read_crc();
        };

        class LIBWUK_API Packet {
        private:
            PacketFlag flag;
            PacketMate mate;
            PacketData data;
            PacketEndianness endian;
            wuk::net::Socket fd;
        public:
            Packet();
            Packet(wuk::net::Socket fd);
        };
    }
}

#endif
#endif
