#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT

namespace wuk {
    namespace color {
        namespace all {
            inline const char *reset   = "\x1b[0m";
            inline const char *flicker = "\x1b[5m"; // 闪烁的字符
        }

        namespace fore {
            inline const char *black        = "\x1b[30m";
            inline const char *red          = "\x1b[31m";
            inline const char *green        = "\x1b[32m";
            inline const char *yellow       = "\x1b[33m";
            inline const char *blue         = "\x1b[34m";
            inline const char *magenta      = "\x1b[35m";
            inline const char *cyan         = "\x1b[36m";
            inline const char *white        = "\x1b[37m";

            inline const char *lightBlack   = "\x1b[90m";
            inline const char *lightRed     = "\x1b[91m";
            inline const char *lightGreen   = "\x1b[92m";
            inline const char *lightYellow  = "\x1b[93m";
            inline const char *lightBlue    = "\x1b[94m";
            inline const char *lightMagenta = "\x1b[95m";
            inline const char *lightCyan    = "\x1b[96m";
            inline const char *lightWhite   = "\x1b[97m";
        }

        namespace back {
            inline const char *red          = "\x1b[41m";
            inline const char *green        = "\x1b[42m";
            inline const char *yellow       = "\x1b[43m";
            inline const char *blue         = "\x1b[44m";
            inline const char *magenta      = "\x1b[45m";
            inline const char *cyan         = "\x1b[46m";
            inline const char *white        = "\x1b[47m";

            inline const char *lightBlack   = "\x1b[100m";
            inline const char *lightRed     = "\x1b[101m";
            inline const char *lightGreen   = "\x1b[102m";
            inline const char *lightYellow  = "\x1b[103m";
            inline const char *lightBlue    = "\x1b[104m";
            inline const char *lightMagenta = "\x1b[105m";
            inline const char *lightCyan    = "\x1b[106m";
            inline const char *lightWhite   = "\x1b[107m";
        }
    }
}

#endif

