template <typename T>
T *wuk::Buffer::write(wuk::ulong length)
{
    if ((this->m_size < length) && !this->is_memory_sufficient(length)) {
        this->expand_memory(length);
    }
    this->m_len = length;
    this->m_offset = this->m_data + length;
    return reinterpret_cast<T *>(this->m_data);
}

template <typename T>
T *wuk::Buffer::append(wuk::ulong length)
{
    if (!this->is_memory_sufficient(length)) {
        this->expand_memory(length);
    }

    this->m_len += length;
    this->m_offset += length;

    return reinterpret_cast<T *>(this->m_offset - length);
}
