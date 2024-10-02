#include <WukBuffer.hh>

wuk::Buffer::Buffer(wByte *data, w_size length)
: data(), length()
{
    if(!data) {
        data = (wByte *)malloc(length);
        if(!data) {
            wuk::Exception(wukErr_ErrMemory, "", "");
        }
    }
}
