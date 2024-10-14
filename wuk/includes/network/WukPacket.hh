#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_PACKET
#define WUK_CPP_PACKET

#include <config/WukException.hh>
#include <network/WukSocket.hh>

#include <WukMemory.hh>
#include <WukBuffer.hh>
#include <WukTime.hh>

#include <zlib.h>

namespace wuk {
    namespace net {
        typedef enum {
            P_IS_NONE    = 0x00000000U, // ................................ 无标志位
            P_IS_FILE    = 0x00000001U, // ...............................1 文件
            P_IS_DATA    = 0x00000002U, // ..............................1. 一般数据
            P_IS_SEGMENT = 0x00000004U, // .............................1.. 已分段
            P_IS_OVER    = 0x00000008U, // ............................1... 已是分段包的最后一个
            P_IS_XCRYPT  = 0x00000010U, // ...........................1.... 已加密
            P_IS_MESSAGE = 0x00000020U, // ..........................1..... 纯消息
            P_IS_URGENT  = 0x00000040U, // .........................1...... 紧急包

            P_IS_IMAGE   = 0x00000080U, // ........................1....... 图像且IS_FILE，否则视为表情包
            P_IS_VIDEO   = 0x00000100U, // .......................1........ 视频
            P_IS_AUDIO   = 0x00000200U, // ......................1......... 音频

            P_IS_ANYM    = 0x00000400U, // .....................1.......... 匿名包

            P_IS_NOERR   = 0x80000000U  // 1............................... 校验位，必须为1，否则弃包
        } MessageTypes;

        class LIBWUK_API PacketType {
        private:
            w_u32 _val;

        public:
            PacketType();
            PacketType(MessageTypes type);

            void set_type_file();
            void set_type_data();
            void set_type_segment();
            void set_type_over();
            void set_type_xcrypt();
            void set_type_msg();
            void set_type_urgent();
            void set_type_image();
            void set_type_video();
            void set_type_audio();
            void set_type_anym();
            void set_type_noerr();

            void clean_type();
            w_u32 get_type_val();
        };

        class LIBWUK_API PacketEndian {
        protected:
            void reverse_array(wByte *array, wSize size);
        
            template <typename T>
            void write_bytearray(wByte *array, wSize array_size, T val);

            template <typename T>
            T read_bytearray(wByte *array);
        };

        class LIBWUK_API Packet {
        private:
            w_u32 p_proto_ver;
            w_u32 p_msg_type;
            w_u32 p_sequence;
            w_u32 p_segment_id;

            double p_time_stamp;

            wSize p_message_id;
            wSize p_message_size;

            wSize p_sender_id;
            wSize p_recipient_id;

            wuk::net::Socket fd;

        public:
            // 元数据
            wByte proto_ver[4];
            wByte msg_type[4];
            wByte sequence[4];
            wByte segment_id[4];

            // 头部数据
            wByte time_stamp[8];
            wByte message_id[8];
            wByte message_size[8];

            wByte sender_id[8];
            wByte recipient_id[8];

            // 实际内容
            wuk::Buffer message;

            // 整个包的摘要
            wByte *sha256_digest[32];

            Packet();
            Packet(wuk::net::Socket fd);

            void reset_socket(wuk::net::Socket &fd);

            void set_mate_info();
            void set_header_info();
        };
    }
}

#endif
#endif
