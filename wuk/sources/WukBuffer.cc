#include <WukBuffer.hh>

wuk::Buffer::Buffer()
: data(nullptr), size()
{
    
}

wuk::Buffer::~Buffer()
{
    free(this->data);
    this->data = nullptr;
}

void wuk::Buffer::append(const wByte *content, wSize length)
{
    this->size += length;

    if(this->data) {
        wByte *new_ptr = (wByte *)realloc(this->data, this->size);

        if(!new_ptr) {
            throw wuk::Exception(wukErr_ErrMemory, "wuk::Buffer::append",
                "Failed to allocate memory for new_ptr.");
        }

        this->data = new_ptr;
    } else {
        this->data = (wByte *)malloc(this->size);

        if(!this->data) {
            throw wuk::Exception(wukErr_ErrMemory, "wuk::Buffer::append",
                "Failed to allocate memory for this->data.");
        }
    }

    memcpy(this->data + this->size - length, content, length);
}

void wuk::Buffer::append(std::string content)
{
    this->append((wByte *)content.c_str(), content.size());
}


