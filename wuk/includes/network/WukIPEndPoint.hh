#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <network/WukNetwork.hh>

namespace wuk {
    namespace net {
        class LIBWUK_API Socket;
    }
}

namespace wuk {
    namespace net {
        // 此类仍需优化，get_host与get_port方法。
        class LIBWUK_API IPEndPoint {
        private:
            SOCKADDR *ai_addr;
            socklen_t ai_addrlen;

            friend class wuk::net::Socket;

        private:
            std::string _to_string_addr(wI32 family, const void *pAddr) const;

            std::string to_string_addr(const SOCKADDR_IN *ipv4_addr) const;
            std::string to_string_addr(const SOCKADDR_IN6 *ipv6_addr) const;
            std::string to_string_addr(const ADDRINFO *addr) const;
            std::string to_string_addr(const SOCKADDR *addr, wI32 family) const;

        public:
            IPEndPoint();
            IPEndPoint(const IPEndPoint &other);
            IPEndPoint(IPEndPoint &&other);
            ~IPEndPoint();

            IPEndPoint(SOCKADDR *addr, socklen_t addrlen);
            IPEndPoint(std::string addr, wU16 port);

            IPEndPoint &operator=(const IPEndPoint &other);
            IPEndPoint &operator=(IPEndPoint &&other) noexcept;

        protected:
            SOCKADDR *get_ai_addr_ptr();
            socklen_t *get_ai_addrlen_ptr();

        public:
            const SOCKADDR *get_ai_addr() const;
            socklen_t get_ai_addrlen() const;
            std::string get_host(wI32 family = AF_INET) const;
            wU16 get_port(wI32 family = AF_INET) const;
        };
    }
}

#endif
