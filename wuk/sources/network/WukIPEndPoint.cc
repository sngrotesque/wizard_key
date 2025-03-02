#include <network/WukIPEndPoint.hh>

std::string wuk::net::IPEndPoint::_to_string_addr(wI32 family, const void *pAddr) const
{
    char buffer_addr[INET6_ADDRSTRLEN]{};

    if (!inet_ntop(family, pAddr, buffer_addr, INET6_ADDRSTRLEN)) {
        // wuk::net::exception("wuk::net::IPEndPoint::to_string_addr");
        throw wuk::net::Exception("wuk::net::IPEndPoint::_to_string_addr");
    }

    return std::string{buffer_addr};
}

std::string wuk::net::IPEndPoint::to_string_addr(const SOCKADDR_IN *ipv4_addr) const
{
    return this->_to_string_addr(AF_INET, &ipv4_addr->sin_addr);
}

std::string wuk::net::IPEndPoint::to_string_addr(const SOCKADDR_IN6 *ipv6_addr) const
{
    return this->_to_string_addr(AF_INET6, &ipv6_addr->sin6_addr);
}

std::string wuk::net::IPEndPoint::to_string_addr(const ADDRINFO *addr) const
{
    if (addr->ai_family == AF_INET) {
        const SOCKADDR_IN *ipv4_addr = reinterpret_cast<SOCKADDR_IN *>(addr->ai_addr);
        return this->to_string_addr(ipv4_addr);
    }
    const SOCKADDR_IN6 *ipv6_addr = reinterpret_cast<SOCKADDR_IN6 *>(addr->ai_addr);
    return this->to_string_addr(ipv6_addr);
}

std::string wuk::net::IPEndPoint::to_string_addr(const SOCKADDR *addr, wI32 family) const
{
    if (family == AF_INET) {
        const SOCKADDR_IN *ipv4_addr = reinterpret_cast<const SOCKADDR_IN *>(addr);
        return this->to_string_addr(ipv4_addr);
    }
    const SOCKADDR_IN6 *ipv6_addr = reinterpret_cast<const SOCKADDR_IN6 *>(addr);
    return this->to_string_addr(ipv6_addr);
}

wuk::net::IPEndPoint::IPEndPoint()
: ai_addr(nullptr), ai_addrlen(sizeof(SOCKADDR_STORAGE))
{
    this->ai_addr = wuk::m_alloc<SOCKADDR *>(this->ai_addrlen);
    if (!this->ai_addr) {
        throw wuk::Exception(wuk::Error::MEMORY, "wuk::net::IPEndPoint::IPEndPoint",
            "Failed to allocate memory for this->ai_addr.");
    }
}

wuk::net::IPEndPoint::IPEndPoint(const wuk::net::IPEndPoint &other)
: ai_addr(nullptr), ai_addrlen(other.ai_addrlen)
{
    this->ai_addr = wuk::m_alloc<SOCKADDR *>(this->ai_addrlen);
    if (!this->ai_addr) {
        throw wuk::Exception(wuk::Error::MEMORY, "wuk::net::IPEndPoint::IPEndPoint",
            "Failed to allocate memory for this->ai_addr.");
    }
    memcpy(this->ai_addr, other.ai_addr, other.ai_addrlen);
}

wuk::net::IPEndPoint::IPEndPoint(wuk::net::IPEndPoint &&other)
: ai_addr(nullptr), ai_addrlen(other.ai_addrlen)
{
    this->ai_addr = other.ai_addr;
    other.ai_addr = nullptr;
}

wuk::net::IPEndPoint::~IPEndPoint()
{
    wuk::m_free(this->ai_addr);
}

wuk::net::IPEndPoint::IPEndPoint(SOCKADDR *addr, socklen_t addrlen)
: ai_addr(nullptr), ai_addrlen(addrlen)
{
    this->ai_addr = wuk::m_alloc<SOCKADDR *>(addrlen);
    if (!this->ai_addr) {
        throw wuk::Exception(wuk::Error::MEMORY, "wuk::net::IPEndPoint::IPEndPoint",
            "Failed to allocate memory for this->ai_addr.");
    }

    memcpy(this->ai_addr, addr, addrlen);
}

wuk::net::IPEndPoint::IPEndPoint(std::string addr, wU16 port)
: ai_addr(nullptr), ai_addrlen()
{
    SOCKADDR_IN6 addrv6{};
    SOCKADDR_IN addrv4{};

    socklen_t addrlen{0};
    SOCKADDR_STORAGE addr_storage{};

    if (addr.find(':') != std::string::npos) {
        addrv6.sin6_family = AF_INET6;
        addrv6.sin6_port = htons(port);
        if (inet_pton(AF_INET6, addr.c_str(), &addrv6.sin6_addr) != 1) {
            throw wuk::net::Exception("wuk::net::IPEndPoint::IPEndPoint");
        }
        this->ai_addr = wuk::m_alloc<SOCKADDR *>(sizeof(addrv6));
        if (!this->ai_addr) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::net::IPEndPoint::IPEndPoint",
                "Failed to allocate memory for this->ai_addr.");
        }
        this->ai_addrlen = sizeof(addrv6);
        memcpy(this->ai_addr, &addrv6, sizeof(addrv6));
    } else {
        addrv4.sin_family = AF_INET;
        addrv4.sin_port = htons(port);
        if (inet_pton(AF_INET, addr.c_str(), &addrv4.sin_addr) != 1) {
            throw wuk::net::Exception("wuk::net::IPEndPoint::IPEndPoint");
        }
        this->ai_addr = wuk::m_alloc<SOCKADDR *>(sizeof(addrv4));
        if (!this->ai_addr) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::net::IPEndPoint::IPEndPoint",
                "Failed to allocate memory for this->ai_addr.");
        }
        this->ai_addrlen = sizeof(addrv4);
        memcpy(this->ai_addr, &addrv4, sizeof(addrv4));
    }


}

wuk::net::IPEndPoint &wuk::net::IPEndPoint::operator=(const wuk::net::IPEndPoint &other)
{
    if (this == &other) {
        return *this;
    }
    wuk::m_free(this->ai_addr);
    this->ai_addrlen = other.ai_addrlen;

    this->ai_addr = wuk::m_alloc<SOCKADDR *>(this->ai_addrlen);
    if (!this->ai_addr) {
        throw wuk::Exception(wuk::Error::MEMORY, "wuk::net::IPEndPoint::IPEndPoint",
            "Failed to allocate memory for this->ai_addr.");
    }
    memcpy(this->ai_addr, other.ai_addr, other.ai_addrlen);

    return *this;
}

wuk::net::IPEndPoint &wuk::net::IPEndPoint::operator=(wuk::net::IPEndPoint &&other) noexcept
{
    if (this == &other) {
        return *this;
    }
    wuk::m_free(this->ai_addr);
    this->ai_addr = other.ai_addr;
    this->ai_addrlen = other.ai_addrlen;

    other.ai_addr = nullptr;
    other.ai_addrlen = 0;

    return *this;
}

SOCKADDR *wuk::net::IPEndPoint::get_ai_addr_ptr()
{
    return this->ai_addr;
}

socklen_t *wuk::net::IPEndPoint::get_ai_addrlen_ptr()
{
    return &this->ai_addrlen;
}

const SOCKADDR *wuk::net::IPEndPoint::get_ai_addr() const
{
    return this->ai_addr;
}

socklen_t wuk::net::IPEndPoint::get_ai_addrlen() const
{
    return this->ai_addrlen;
}

std::string wuk::net::IPEndPoint::get_host(wI32 family) const
{
    if (this->ai_addrlen == sizeof(SOCKADDR_IN)) {
        return this->to_string_addr(this->ai_addr, AF_INET);
    } else if (this->ai_addrlen == sizeof(SOCKADDR_IN6)) {
        return this->to_string_addr(this->ai_addr, AF_INET6);
    } else {
        throw wuk::net::Exception(wuk::Error::ERR, "wuk::net::IPEndPoint::get_host",
            "This protocol is currently not supported.");
    }
}

wU16 wuk::net::IPEndPoint::get_port(wI32 family) const
{
    if (this->ai_addrlen == sizeof(SOCKADDR_IN)) {
        SOCKADDR_IN *ipv4 = reinterpret_cast<SOCKADDR_IN *>(this->ai_addr);
        return ntohs(ipv4->sin_port);
    } else if (this->ai_addrlen == sizeof(SOCKADDR_IN6)) {
        SOCKADDR_IN6 *ipv6 = reinterpret_cast<SOCKADDR_IN6 *>(this->ai_addr);
        return ntohs(ipv6->sin6_port);
    } else {
        throw wuk::net::Exception(wuk::Error::ERR, "wuk::net::IPEndPoint::get_host",
            "This protocol is currently not supported.");
    }
    return 0;
}
