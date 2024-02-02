/*
* ### Big-endian
* 
* 1. packet seq:    4 Bytes.
* 2. packet length: 4 Bytes.
* 3. packet crc32:  4 Bytes (seq + length).
* 4. packet data:   [packet length] Bytes.
* 5. packet sha256: 32 Bytes.
* 6. packet end:    8 Bytes. [53 4e 45 03 4e 04 45 05]
*/
#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_PACKET
#define WMKC_CPP_PACKET

#include <network/wmkc_net.hpp>
#include <wmkc_struct.hpp>
#include <wmkc_hash.hpp>

#include <zlib.h>

#define WMKC_PACKET_END "\x53\x4e\x45\x03\x4e\x04\x45\x05"
#define WMKC_PACKET_END_LEN 8

namespace wmkcNet {
    class wmkcPacket {
        private:
            wmkcNet::Sockfd fd;

            // order: [Seq>Length>CRC>Data>Digest>End]
            wmkcByte digest[32]; // SHA-256 Digest
            wmkc_u32 _;          // nothing...just pad
            wmkc_u32 seq;        // packet seq
            wmkc_u32 crc;        // [seq + length] crc
            wmkc_u32 length;     // packet data length
            wmkcByte *data;      // packet data
            wmkcByte end[8];     // packet end
        
        public:
            wmkcPacket(wmkcNet::Socket current_fd);
            wmkcVoid send(std::string content);
            std::string recv();
    };
};

#endif
#endif
