#include <network/WukPacket.hh>

// wuk::net::PacketFlag Begin
wuk::net::PacketFlag::PacketFlag()
: packet_flag_value()
{
    this->set_flag_noerr();
}

// 单独设置
void wuk::net::PacketFlag::set_flag_file()
{
    this->packet_flag_value |= PACKET_IS_FILE;
}

void wuk::net::PacketFlag::set_flag_data()
{
    this->packet_flag_value |= PACKET_IS_DATA;
}

void wuk::net::PacketFlag::set_flag_segment()
{
    this->packet_flag_value |= PACKET_IS_SEGMENT;
}

void wuk::net::PacketFlag::set_flag_over()
{
    this->packet_flag_value |= PACKET_IS_OVER;
}

void wuk::net::PacketFlag::set_flag_xcrypt()
{
    this->packet_flag_value |= PACKET_IS_XCRTPT;
}

void wuk::net::PacketFlag::set_flag_msg()
{
    this->packet_flag_value |= PACKET_IS_MSG;
}

void wuk::net::PacketFlag::set_flag_urgent()
{
    this->packet_flag_value |= PACKET_IS_URGENT;
}

void wuk::net::PacketFlag::set_flag_noerr()
{
    this->packet_flag_value |= PACKET_IS_NOERR;
}

// 统一设置
void wuk::net::PacketFlag::clean_flag()
{
    this->packet_flag_value = PACKET_IS_NONE;
}

void wuk::net::PacketFlag::set_flag(PacketFlagValues flag)
{
    this->packet_flag_value = flag;
}

void wuk::net::PacketFlag::set_flag(wU32 flag)
{
    this->packet_flag_value = (flag & 0xff);
}
// wuk::net::PacketFlag End

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
: mate_time(),  mate_session_id(), mate_sequence(), mate_crc()
{

}

// 写入元数据：时间
void wuk::net::PacketMate::write_mate_time(double current_time)
{
    this->write_bytearray(this->mate_time, sizeof(this->mate_time), current_time);
}

// 写入元数据：会话ID
void wuk::net::PacketMate::write_mate_session_id(w_ulong id)
{
    this->write_bytearray(this->mate_session_id, sizeof(this->mate_session_id), id);
}

// 写入元数据：顺序号
void wuk::net::PacketMate::write_mate_sequence(w_u32 seq)
{
    this->write_bytearray(this->mate_sequence, sizeof(this->mate_sequence), seq);
}

// 写入元数据：CRC32校验值
void wuk::net::PacketMate::write_mate_crc()
{
    w_u32 crc_val{};

    crc_val = crc32(crc_val, this->mate_time, sizeof(this->mate_time));
    crc_val = crc32(crc_val, this->mate_session_id, sizeof(this->mate_session_id));
    crc_val = crc32(crc_val, this->mate_sequence, sizeof(this->mate_sequence));

    this->write_bytearray(this->mate_crc, sizeof(this->mate_crc), crc_val);
}

// 读取元数据：时间
double wuk::net::PacketMate::read_mate_time()
{
    return this->read_bytearray<double>(this->mate_time);
}

// 读取元数据：会话ID
w_ulong wuk::net::PacketMate::read_mate_session_id()
{
    return this->read_bytearray<wSize>(this->mate_session_id);
}

// 读取元数据：顺序号
w_u32 wuk::net::PacketMate::read_mate_sequence()
{
    return this->read_bytearray<w_u32>(this->mate_sequence);
}

// 读取元数据：CRC32校验值
w_u32 wuk::net::PacketMate::read_mate_crc()
{
    return this->read_bytearray<w_u32>(this->mate_crc);
}
// wuk::net::PacketMate End


// wuk::net::PacketData Begin
wuk::net::PacketData::PacketData()
: /* length_val(),*/ data_data(), data_size(), data_crc()
{

}

void wuk::net::PacketData::write_data_data(wByte *data, wU32 length)
{
    if(length > (static_cast<wU32>(~0) - 4)) {
        throw wuk::Exception(wukErr_Err, "wuk::net::PacketData::write_data",
            "The data is too long, please segment it.");
    }

    this->length_val = length;

    // // 将用户指定的数据复制到数据包的数据中
    // this->data_data = (wByte *)malloc(this->length_val);
    // if(!this->data_data) {
    //     throw wuk::Exception(wukErr_ErrMemory, "wuk::net::PacketData::write_data",
    //         "Failed to allocate memory for data_data.");
    // }
    // memcpy(this->data_data, data, this->length_val);

    /*
    * 将数据包数据指针指向用户指定数据（不确定此行为是否存在问题）
    * 因为像std::string类型存在析构函数，可能会在某些非需要的情况下回收指向的数据。
    * 此行为应在现在以及未来进行监测，同时保留destroy_data_data方法。
    */ 
    this->data_data = data;

    this->write_bytearray(this->data_size, sizeof(this->data_size), this->length_val);
}

void wuk::net::PacketData::write_data_data(std::string data)
{
    wByte *p = reinterpret_cast<wByte *>(const_cast<char *>(data.c_str()));
    this->write_data_data(p, data.size());
}

void wuk::net::PacketData::write_data_crc()
{
    if(!this->data_data) {
        throw wuk::Exception(wukErr_ErrNULL, "wuk::net::PacketData::write_crc",
            "this->data_data is nullptr.");
    }
    w_u32 crc_val{0};

    crc_val = crc32(crc_val, this->data_size, sizeof(this->data_size));
    crc_val = crc32(crc_val, this->data_data, this->length_val);

    this->write_bytearray(this->data_crc, sizeof(this->data_crc), crc_val);
}

wByte *wuk::net::PacketData::read_data_data()
{
    return this->data_data;
}

w_u32 wuk::net::PacketData::read_data_length()
{
    return this->length_val;
}

w_u32 wuk::net::PacketData::read_data_crc()
{
    return this->read_bytearray<w_u32>(this->data_crc);
}

// void wuk::net::PacketData::destroy_data_data()
// {
//     free(this->data_data);
//     this->data_data = nullptr;
// }
// wuk::net::PacketData End


wuk::net::Packet::Packet()
: flag(), mate(), data(), endian(), fd()
{

}

wuk::net::Packet::Packet(wuk::net::Socket fd)
: flag(), mate(), data(), endian(), fd(fd)
{

}

void wuk::net::Packet::set_packet_flag(PacketFlag _flag)
{
    this->flag = _flag;
}

void wuk::net::Packet::set_packet_mate(PacketMate _mate)
{
    this->mate = _mate;
}

void wuk::net::Packet::set_packet_mate(double current_time, wSize session_id, w_u32 seq)
{
    this->mate.write_mate_time(current_time);
    this->mate.write_mate_session_id(session_id);
    this->mate.write_mate_sequence(seq);
}

void wuk::net::Packet::set_packet_data(PacketData _data)
{
    this->data = _data;
}

void wuk::net::Packet::set_packet_data(wByte *data, w_u32 data_length)
{
    this->data.write_data_data(data, data_length);
}

void wuk::net::Packet::set_packet_data(std::string data)
{
    this->data.write_data_data(data);
}

wuk::Buffer wuk::net::Packet::build_packet_data()
{
    wuk::Buffer packet{};

    // 位标志
    packet.append(&this->flag.packet_flag_value, 1);

    // 元数据
    packet.append(this->mate.mate_time, sizeof(this->mate.mate_time));
    packet.append(this->mate.mate_session_id, sizeof(this->mate.mate_session_id));
    packet.append(this->mate.mate_sequence, sizeof(this->mate.mate_sequence));
    packet.append(this->mate.mate_crc, sizeof(this->mate.mate_crc));

    // 实际数据
    packet.append(this->data.data_size, sizeof(this->data.data_size));
    packet.append(this->data.data_data, this->data.length_val);
    packet.append(this->data.data_crc, sizeof(this->data.data_crc));

    return packet;
}

void wuk::net::Packet::analyze_packet(wuk::Buffer packet_data)
{
    
}
