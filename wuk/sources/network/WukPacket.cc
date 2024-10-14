#include <network/WukPacket.hh>

///////////////////////////////////////////////////////////////////
wuk::net::PacketType::PacketType()
: _val(P_IS_NOERR)
{

}

wuk::net::PacketType::PacketType(MessageTypes type)
: _val(type)
{
    
}

void wuk::net::PacketType::set_type_file()
{
    this->_val |= P_IS_FILE;
}

void wuk::net::PacketType::set_type_data()
{
    this->_val |= P_IS_DATA;
}

void wuk::net::PacketType::set_type_segment()
{
    this->_val |= P_IS_SEGMENT;
}

void wuk::net::PacketType::set_type_over()
{
    this->_val |= P_IS_OVER;
}

void wuk::net::PacketType::set_type_xcrypt()
{
    this->_val |= P_IS_XCRYPT;
}

void wuk::net::PacketType::set_type_msg()
{
    this->_val |= P_IS_MESSAGE;
}

void wuk::net::PacketType::set_type_urgent()
{
    this->_val |= P_IS_URGENT;
}

void wuk::net::PacketType::set_type_image()
{
    this->_val |= P_IS_IMAGE;
}

void wuk::net::PacketType::set_type_video()
{
    this->_val |= P_IS_VIDEO;
}

void wuk::net::PacketType::set_type_audio()
{
    this->_val |= P_IS_AUDIO;
}

void wuk::net::PacketType::set_type_anym()
{
    this->_val |= P_IS_ANYM;
}

void wuk::net::PacketType::set_type_noerr()
{
    this->_val |= P_IS_NOERR;
}

void wuk::net::PacketType::clean_type()
{
    this->_val = P_IS_NONE;
}

w_u32 wuk::net::PacketType::get_type_val()
{
    return this->_val;
}

///////////////////////////////////////////////////////////////////
void wuk::net::PacketEndian::reverse_array(wByte *array, wSize size)
{
    for(w_u32 i = 0; i < (size >> 1); ++i) {
        wByte swap = array[i];
        array[i] = array[size - i - 1];
        array[size - i - 1] = swap;
    }
}

template <typename T>
void wuk::net::PacketEndian::write_bytearray(wByte *array, wSize array_size, T val)
{
    if (!array) {
        throw wuk::Exception(wukErr_ErrNULL, "wuk::net::PacketEndian::write_bytearray",
            "array is nullptr.");
    }
    
    if (sizeof(T) > array_size) {
        throw wuk::Exception(wukErr_Err, "wuk::net::PacketEndian::write_bytearray",
            "The data size exceeds the size of the array.");
    }

    memcpy(array, &val, sizeof(T));

    if (WUK_LE_ENDIAN) {
        this->reverse_array(array, sizeof(T));
    }
}

template <typename T>
T wuk::net::PacketEndian::read_bytearray(wByte *array)
{
    T val;

    if (WUK_LE_ENDIAN) {
        this->reverse_array(array, sizeof(T));
    }

    memcpy(&val, array, sizeof(T));

    return val;
}

///////////////////////////////////////////////////////////////////


