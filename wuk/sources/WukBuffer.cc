#include <WukBuffer.hh>

//////////////////////////////////////////////////////////////////////
wuk::Buffer::Buffer()
: data(nullptr), size()
{
    
}

wuk::Buffer::Buffer(wByte *content, wSize length)
: data(nullptr), size(length)
{
    this->data = (wByte *)malloc(length);
    if (!this->data) {
        throw wuk::Exception(wukErr_ErrMemory, "wuk::Buffer::Buffer",
            "Failed to allocate memory for this->data.");
    }

    memcpy(this->data, content, length);
}

wuk::Buffer::Buffer(wSize memory_size)
: data(nullptr), size(memory_size)
{
    this->data = (wByte *)malloc(memory_size);
    if (!this->data) {
        throw wuk::Exception(wukErr_ErrMemory, "wuk::Buffer::Buffer",
            "Failed to allocate memory for this->data.");
    }
}

wuk::Buffer::~Buffer()
{
    free(this->data);
    this->data = nullptr;
}

//////////////////////////////////////////////////////////////////////
void wuk::Buffer::write(wByte *content, wSize length)
{

}

wByte *wuk::Buffer::write(wSize length)
{
    
}

void wuk::Buffer::append(const wByte *content, wSize length)
{
    this->size += length;

    if(this->data) {
        // 如果内部数据已经被使用
        wByte *new_ptr = (wByte *)realloc(this->data, this->size);

        if(!new_ptr) {
            throw wuk::Exception(wukErr_ErrMemory, "wuk::Buffer::append",
                "Failed to allocate memory for new_ptr.");
        }

        this->data = new_ptr;
    } else {
        // 如果内部数据还未被使用
        this->data = (wByte *)malloc(this->size);

        if(!this->data) {
            throw wuk::Exception(wukErr_ErrMemory, "wuk::Buffer::append",
                "Failed to allocate memory for this->data.");
        }
    }

    // 将新数据加入内部数据
    memcpy(this->data + this->size - length, content, length);
}

void wuk::Buffer::append(std::string content)
{
    this->append((wByte *)content.c_str(), content.size());
}

wByte *wuk::Buffer::get_data()
{
    return this->data;
}

wSize wuk::Buffer::get_size()
{
    return this->size;
}
