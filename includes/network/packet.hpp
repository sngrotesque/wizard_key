/*
* 此C++代码的一切请参考自己编写的Python代码中的packet.py进行编写
*/

#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_PACKET
#define WMKC_CPP_PACKET

#include <network/socket.hpp>
#include <crypto/hashlib.hpp>
#include <struct.hpp>

#include <zlib.h>
#include <lzma.h>

namespace wmkc {
    namespace net {
        class Packet {
            private:
                net::Socket fd;

            public:
                Packet(net::Socket current_fd);
                void send(std::string content);
                std::string recv();
        };
    };
}

#endif
#endif
