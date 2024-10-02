// python make.py test\packet.cpp -O3 -Wall -DWUK_EXPORTS -lz
#include <config/wuk.hpp>
#include <random.cpp>
#include <time.cpp>
#include <misc.cpp>

#include <iostream>
#include <sstream>
#include <vector>
#include <any>
#include <cstdarg>
#include <cmath>

#include <zlib.h>

class PacketBuffer {
public:
    wByte *c_bytes;
    wSize c_length;

    PacketBuffer()
    : c_bytes(), c_length()
    {

    }

    PacketBuffer(wByte *data, wSize size)
    : c_bytes(data), c_length(size)
    {

    }

    void destroy()
    {
        free(this->c_bytes);
        this->c_bytes = nullptr;
    }
};

namespace wuk {
    class LIBWUK_API Packet {
    public:
        void reverse_array(wByte *array, w_u32 size)
        {
            for(w_u32 i = 0; i < (size >> 1); ++i) {
            wByte swap = array[i];
            array[i] = array[size - i - 1];
            array[size - i - 1] = swap;
            }
        }

        template <typename T>
        void get_buffer_for_number(wByte *buffer, T arg)
        {
            memcpy(buffer, &arg, sizeof(T));
            if(WUK_LE_ENDIAN) {
                this->reverse_array(buffer, sizeof(T));
            }
        }

        PacketBuffer create_packet(w_u32 seq, w_ulong session_id, double time_stamp)
        {
            wByte *first_packet = nullptr;
            wByte *p = nullptr;
            PacketBuffer result;

            result.c_length = 4 + 8 + 8 + 4;

            first_packet = (wByte *)malloc(result.c_length);
            if(!first_packet) {
                throw wuk::Exception(wukErr_ErrMemory, "create_packet",
                    "Failed to allocate memory for first_packet.");
            }

            p = first_packet;

            this->get_buffer_for_number(p, seq);
            p += 4;

            this->get_buffer_for_number(p, session_id);
            p += 8;

            this->get_buffer_for_number(p, time_stamp);
            p += 8;

            w_u32 crc_val = crc32(0, first_packet, result.c_length - 4);
            this->get_buffer_for_number(p, crc_val);

            result.c_bytes = first_packet;

            return result;
        }
    };
}

int main(int argc, char **argv)
{
    const char *const_string = "bro...";
    PacketBuffer buffer{
        reinterpret_cast<wByte *>(const_cast<char *>(const_string)),
        strlen(const_string)
    };
    PacketBuffer result;
    wuk::Packet packet;
    wuk::Random random;

    try {
        result = packet.create_packet(
            random.randint(0, ~0U),
            random.randint(0, ~0ULL),
            wuk::Time().time());
        wuk::misc::print_hex(result.c_bytes, result.c_length, 32, 1, 0);

        result.destroy();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
