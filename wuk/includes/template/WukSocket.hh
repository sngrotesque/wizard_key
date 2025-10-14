template <typename T>
void wuk::net::Socket::setsockopt(wuk::i32 level, wuk::i32 opt_name, const T &value)
{
    const char *opt_ptr = reinterpret_cast<const char *>(&value);
    socklen_t opt_len = static_cast<socklen_t>(sizeof(T));

    int err = ::setsockopt(this->m_fd, level, opt_name, opt_ptr, opt_len);

    if (err == NETERROR) {
        int err_code = wuk::net::err::system::code();
        throw wuk::Exception(err_code, "wuk::net::Socket::setsockopt",
            wuk::net::err::system::message(err_code));
    }
}

template <typename T>
T wuk::net::Socket::getsockopt(wuk::i32 level, wuk::i32 opt_name)
{
    T value {};

    char *opt_ptr = reinterpret_cast<char *>(&value);
    socklen_t opt_len = static_cast<socklen_t>(sizeof(T));

    wuk::i32 err = ::getsockopt(this->m_fd, level, opt_name, opt_ptr, &opt_len);

    if (opt_name == SO_ERROR) {
        return value;
    }
    if (err == NETERROR) {
        wuk::i32 err_code = wuk::net::err::system::code();
        throw wuk::Exception(err_code, "wuk::net::Socket::getsockopt",
            wuk::net::err::system::message(err_code));
    }

    return value;
}
