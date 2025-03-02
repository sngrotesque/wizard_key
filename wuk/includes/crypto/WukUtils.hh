#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <WukMemory.hh>

namespace wuk {
    namespace crypto {
        constexpr wU32 COUNTER_BLOCK_SIZE = 16;

        class LIBWUK_API Counter {
        private:
            wByte counter[COUNTER_BLOCK_SIZE]{};

        public:
            Counter() = default;

            Counter(const wByte *nonce, wU32 size, wSize begin);
            Counter(const char *nonce, wU32 size, wSize begin);
            Counter(std::string nonce, wSize begin);

        public:
            wByte *get() noexcept;
            void clean() noexcept;
            void step_up() noexcept;
        };
    }
}

#endif
