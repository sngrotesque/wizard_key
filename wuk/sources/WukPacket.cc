#include <WukPacket.hh>

wuk::Packet::Packet(w_ulong session_id)
: session_id(session_id)
{

}

void wuk::Packet::send(wuk::net::Socket fd, w_byte *data, w_ulong length, w_u32 sequence)
{
    
}




