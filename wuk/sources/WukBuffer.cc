#include <WukBuffer.hh>

//////////////////////////////////////////////////////////////////////
/**
 * @brief 用于增加可用内存大小
 * @authors SN-Grotesque
 * @note 不需要在函数外部重新分配this->data_size大小，此方法会自动完成
 * @param length 需要增加的长度（非总长度），比如要加16字节，就传入16。
 * @return 无
 */
void wuk::Buffer::expand_memory(wSize length)
{
    printf("[TEST] wuk::Buffer::expand_memory: %zd\n", length);
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
        printf("[TEST] wuk::Buffer::expand_memory: %p\n", this->data);

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

/**
 * @brief 检查当前已申请的内存空间是否足够
 * @authors SN-Grotesque
 * @note 无
 * @param length 新数据的长度
 * @return 如果足够就返回True，否则False
 */
bool wuk::Buffer::is_memory_sufficient(wSize length)
{
    return (this->data_len + length) > this->data_size;
}

//////////////////////////////////////////////////////////////////////
wuk::Buffer::Buffer()
: data(nullptr), data_offset(nullptr), data_len(), data_size()
{
    
}

wuk::Buffer::Buffer(const wByte *content, wSize length)
: data(nullptr), data_offset(nullptr), data_len(length), data_size(length)
{
    this->data = (wByte *)malloc(this->data_len);
    if (!this->data) {
        throw wuk::Exception(wukErr_ErrMemory, "wuk::Buffer::Buffer",
            "Failed to allocate memory for this->data.");
    }
    memcpy(this->data, content, length);

    this->data_offset = this->data + length;
}

wuk::Buffer::Buffer(const std::string content)
: Buffer(reinterpret_cast<const wByte *>(content.c_str()), content.size())
{
    
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
/**
 * @brief 在需要写入指定长度的大小的内容且同时需要指针的情况下调用此方法
 * @authors SN-Grotesque
 * @note 避免临时开辟临时缓冲区造成资源浪费。
 *       recv(fd, buffer.write(2048), 2048, 0);
 * @param length 将要写入的数据内容的长度
 * @return 指向内部数据内容结尾的指针
 */
wByte *wuk::Buffer::append_write(wSize length)
{
    if(this->is_memory_sufficient(length)) {
        this->expand_memory(length);
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

    if(this->is_memory_sufficient(length)) {
        this->expand_memory(length);
    }

    memcpy(this->data_offset, content, length);

    this->data_offset += length;
    this->data_len += length;
}

void wuk::Buffer::append(const std::string content)
{
    this->append(reinterpret_cast<const wByte *>(content.c_str()),
                content.size());
}

//////////////////////////////////////////////////////////////////////
wByte *wuk::Buffer::get_data() const
{
    return this->data;
}

const char *wuk::Buffer::get_cStr() const
{
    return reinterpret_cast<const char *>(this->data);
}

wSize wuk::Buffer::get_length() const
{
    return this->data_len;
}

wSize wuk::Buffer::get_size() const
{
    return this->data_size;
}
