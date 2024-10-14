#include <WukBuffer.hh>

//////////////////////////////////////////////////////////////////////
void wuk::Buffer::expand_memory(wSize length)
{
    if (!this->data) {
        // 如果指针还未使用
        this->data = (wByte *)malloc(length);
        if (!this->data) {
            throw wuk::Exception(wukErr_ErrMemory, "wuk::Buffer::expand_memory",
                "Failed to allocate memory for this->data.");
        }
        this->data_offset = this->data;
    } else {
        // 如果指针已使用
        wSize offset_value = this->data_offset - this->data;
        wByte *tmp_ptr = (wByte *)realloc(this->data, this->data_size + length);
        if (!tmp_ptr) {
            throw wuk::Exception(wukErr_ErrMemory, "wuk::Buffer::expand_memory",
                "Expanding memory size failed.");
        }
        this->data = tmp_ptr;
        this->data_offset = this->data + offset_value;
    }

    this->data_size += length;
}

//////////////////////////////////////////////////////////////////////
wuk::Buffer::Buffer()
: data(nullptr), data_offset(nullptr), data_len(), data_size()
{
    
}

wuk::Buffer::Buffer(wByte *content, wSize length)
: data(nullptr), data_offset(nullptr), data_len(length), data_size(length)
{
    this->data = (wByte *)malloc(this->data_len);
    if (!this->data) {
        throw wuk::Exception(wukErr_ErrMemory, "wuk::Buffer::Buffer",
            "Failed to allocate memory for this->data.");
    }
    memcpy(this->data, content, length);

    this->data_offset = this->data;
}

wuk::Buffer::Buffer(wSize memory_size)
: data(nullptr), data_offset(nullptr), data_len(), data_size(memory_size)
{
    this->data = (wByte *)malloc(this->data_size);
    if (!this->data) {
        throw wuk::Exception(wukErr_ErrMemory, "wuk::Buffer::Buffer",
            "Failed to allocate memory for this->data.");
    }

    wuk::memory_zero(this->data, this->data_size);

    this->data_offset = this->data;
}

wuk::Buffer::~Buffer()
{
    free(this->data);
    this->data = nullptr;
}

//////////////////////////////////////////////////////////////////////
void wuk::Buffer::write(wByte *content, wSize length)
{
    if(!content) {
        throw wuk::Exception(wukErr_ErrNULL, "wuk::Buffer::write",
            "content is nullptr.");
    }

    if(this->data_offset < this->data) {
        throw wuk::Exception(wukErr_Err, "wuk::Buffer::write",
            "The offset pointer points to an incorrect memory address.");
    }

    if((this->data_len + length) > this->data_size) {
        throw wuk::Exception(wukErr_Err, "wuk::Buffer::write",
            "The amount of data written exceeds the requested memory space size.");
    }

    memcpy(this->data_offset, content, length);

    this->data_offset += length;
    this->data_len    += length;
}

wByte *wuk::Buffer::write(wSize length)
{
    if((this->data_size < length) || ((this->data_len + length) > this->data_size)) {
        throw wuk::Exception(wukErr_Err, "wuk::Buffer::write",
            "The amount of data written exceeds the requested memory space size.");
    }

    this->data_len += length;
    this->data_offset += length;

    return this->data_offset - length;
}

void wuk::Buffer::append(const wByte *content, wSize length)
{
    if(!content) {
        throw wuk::Exception(wukErr_ErrNULL, "wuk::Buffer::append",
            "content in nullptr.");
    }

    if(this->data_offset < this->data) {
        throw wuk::Exception(wukErr_Err, "wuk::Buffer::append",
            "The offset pointer points to an incorrect memory address.");
    }

    if((this->data_len + length) > this->data_size) {
        // 内存空间不足的情况下
    }
}

//////////////////////////////////////////////////////////////////////
wByte *wuk::Buffer::get_data()
{
    return this->data;
}

wSize wuk::Buffer::get_length()
{
    return this->data_len;
}

wSize wuk::Buffer::get_size()
{
    return this->data_size;
}
