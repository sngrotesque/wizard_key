#include <config/wmkc.hpp>

// 后续看看情况是否将这些字符串改为std::string
#if WMKC_SUPPORT
#ifndef WMKC_COLOR
#define WMKC_COLOR

namespace wmkc {
    namespace color {
        namespace all {
            const char *reset   = "\x1b[0m";
            const char *flicker = "\x1b[5m"; // 闪烁的字符
        }

        namespace fore {
            const char *black        = "\x1b[30m";
            const char *red          = "\x1b[31m";
            const char *green        = "\x1b[32m";
            const char *yellow       = "\x1b[33m";
            const char *blue         = "\x1b[34m";
            const char *magenta      = "\x1b[35m";
            const char *cyan         = "\x1b[36m";
            const char *white        = "\x1b[37m";

            const char *lightBlack   = "\x1b[90m";
            const char *lightRed     = "\x1b[91m";
            const char *lightGreen   = "\x1b[92m";
            const char *lightYellow  = "\x1b[93m";
            const char *lightBlue    = "\x1b[94m";
            const char *lightMagenta = "\x1b[95m";
            const char *lightCyan    = "\x1b[96m";
            const char *lightWhite   = "\x1b[97m";
        }

        namespace back {
            const char *red          = "\x1b[41m";
            const char *green        = "\x1b[42m";
            const char *yellow       = "\x1b[43m";
            const char *blue         = "\x1b[44m";
            const char *magenta      = "\x1b[45m";
            const char *cyan         = "\x1b[46m";
            const char *white        = "\x1b[47m";

            const char *lightBlack   = "\x1b[100m";
            const char *lightRed     = "\x1b[101m";
            const char *lightGreen   = "\x1b[102m";
            const char *lightYellow  = "\x1b[103m";
            const char *lightBlue    = "\x1b[104m";
            const char *lightMagenta = "\x1b[105m";
            const char *lightCyan    = "\x1b[106m";
            const char *lightWhite   = "\x1b[107m";
        }
    }
}

#endif /* WMKC_COLOR */
#endif

