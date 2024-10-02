#include <network/WukPacket.hh>

// wuk::net::PacketEndianness Begin
void wuk::net::PacketEndianness::reverse_array(wByte *array, wU32 size)
{
    for(w_u32 i = 0; i < (size >> 1); ++i) {
        wByte swap = array[i];
        array[i] = array[size - i - 1];
        array[size - i - 1] = swap;
    }
}

template <typename T>
void wuk::net::PacketEndianness::write_bytearray(wByte *array, wSize array_size, T val)
{
    if (!array) {
        throw wuk::Exception(wukErr_ErrNULL, "wuk::net::write_bytearray",
            "array is nullptr.");
    }
    
    if (sizeof(T) > array_size) {
        throw wuk::Exception(wukErr_Err, "wuk::net::write_bytearray",
            "The data size exceeds the size of the array.");
    }

    memcpy(array, &val, sizeof(T));

    if (WUK_LE_ENDIAN) {
        this->reverse_array(array, sizeof(T));
    }
}

template <typename T>
T wuk::net::PacketEndianness::read_bytearray(wByte *array)
{
    T val;

    if (WUK_LE_ENDIAN) {
        this->reverse_array(array, sizeof(T));
    }

    memcpy(&val, array, sizeof(T));

    return val;
}
// wuk::net::PacketEndianness End


// wuk::net::PacketMate Begin
wuk::net::PacketMate::PacketMate()
: packet_time(),  session_id(), sequence(), mate_crc()
{

}

void wuk::net::PacketMate::write_time(double current_time)
{

}

void wuk::net::PacketMate::write_session_id(w_ulong id)
{

}

void wuk::net::PacketMate::write_sequence(w_u32 seq)
{

}

void wuk::net::PacketMate::write_mate_crc()
{

}

double wuk::net::PacketMate::read_time()
{

}

w_ulong wuk::net::PacketMate::read_session_id()
{

}

w_u32 wuk::net::PacketMate::read_sequence()
{

}

w_u32 wuk::net::PacketMate::read_mate_crc()
{

}
// wuk::net::PacketMate End


// wuk::net::PacketData Begin
wuk::net::PacketData::PacketData()
: length_val(), data(), length(), dataCRC()
{

}

void wuk::net::PacketData::write_data(wByte *data, wSize data_length)
{

}

void wuk::net::PacketData::write_crc()
{

}

wByte *wuk::net::PacketData::read_data()
{

}

w_u32 wuk::net::PacketData::read_crc()
{

}
// wuk::net::PacketData End


wuk::net::Packet::Packet()
: flag(), mate(), data(), endian(), fd()
{

}

wuk::net::Packet::Packet(wuk::net::Socket fd)
: flag(), mate(), data(), endian(), fd(fd)
{

}
