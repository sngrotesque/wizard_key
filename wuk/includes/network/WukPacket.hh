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
#include <WukBuffer.hh>

#include <zlib.h>

namespace wuk {
    namespace net {
        // 此处不使用`enum class`的目的是方便，同时因为在命名空间中就不容易冲突。
        typedef enum {
            PACKET_IS_NONE    = 0x0,  // 00000000 什么都没有（一般用于清空标志位）
            PACKET_IS_FILE    = 0x1,  // 00000001 此包传输的是文件
            PACKET_IS_DATA    = 0x2,  // 00000010 此包传输的是有效数据
            PACKET_IS_SEGMENT = 0x4,  // 00000100 此包传输的已分段数据包
            PACKET_IS_OVER    = 0x8,  // 00001000 此包是此处传输的最后一个包
            PACKET_IS_XCRTPT  = 0x10, // 00010000 此包已加密
            PACKET_IS_MSG     = 0x20, // 00100000 此包为纯消息包
            PACKET_IS_URGENT  = 0x40, // 01000000 此包为紧急包
            PACKET_IS_NOERR   = 0x80  // 10000000 校验位，必须为1
        } PacketFlagValues;

        class LIBWUK_API PacketFlag {
        public:
            wByte packet_flag_value;

            PacketFlag();

            // 此处为单独设置
            void set_flag_file();
            void set_flag_data();
            void set_flag_segment();
            void set_flag_over();
            void set_flag_xcrypt();
            void set_flag_msg();
            void set_flag_urgent();
            void set_flag_noerr();

            // 此处为统一设置
            void clean_flag();
            void set_flag(PacketFlagValues flag);
            void set_flag(wU32 flag);
        };

        class LIBWUK_API PacketEndianness {
        protected:
            void reverse_array(wByte *array, wU32 size);

            template <typename T>
            void write_bytearray(wByte *array, wSize array_size, T val);

            template <typename T>
            T read_bytearray(wByte *array);
        };

        class LIBWUK_API PacketMate :public PacketEndianness {
        public:
            wByte mate_time[8];
            wByte mate_session_id[8];
            wByte mate_sequence[4];
            wByte mate_crc[4];

            PacketMate();

            void write_mate_time(double current_time);
            void write_mate_session_id(w_ulong id);
            void write_mate_sequence(w_u32 seq);
            void write_mate_crc();

            double read_mate_time();
            wSize  read_mate_session_id();
            w_u32  read_mate_sequence();
            w_u32  read_mate_crc();
        };

        class LIBWUK_API PacketData :public PacketEndianness {
        public:
            /*
            * 这个变量的存在是为了更方便的处理长度数据，并且未来可能会添加
            * 类似于std::string中那样的append方法。
            */
            w_u32 length_val;
            /*
            * 此处将data_data排在data_size之前，是为了内存对齐，
            * 实际使用时，应该将data_size视为第一个数据成员。
            */
            wByte *data_data;
            wByte data_size[4];
            wByte data_crc[4];

            PacketData();

            void write_data_data(wByte *data, wU32 data_length);
            void write_data_data(std::string data);
            void write_data_crc();

            wByte *read_data_data();
            w_u32 read_data_length();
            w_u32 read_data_crc();

            // void destroy_data_data();
        };

        class LIBWUK_API Packet {
        public:
            PacketFlag flag;
            PacketMate mate;
            PacketData data;
            PacketEndianness endian;
            wuk::net::Socket fd;

            Packet();
            Packet(wuk::net::Socket fd);

            // 下列方法是为了在你不想直接使用成员时可以相对方便的赋值
            void set_packet_flag(PacketFlag _flag);

            void set_packet_mate(PacketMate _mate);
            void set_packet_mate(double current_time, wSize session_id, w_u32 seq);

            void set_packet_data(PacketData _data);
            void set_packet_data(wByte *data, w_u32 data_length);
            void set_packet_data(std::string data);

            wuk::Buffer build_packet_data();
        };
    }
}

#endif
#endif
