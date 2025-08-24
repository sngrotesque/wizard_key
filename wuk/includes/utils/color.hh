#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT

namespace wuk::color {
    namespace all {
        inline constexpr char reset[]        = "\x1b[0m";
        inline constexpr char flicker[]      = "\x1b[5m"; // 闪烁的字符
    }

    namespace fore {
        inline constexpr char black[]        = "\x1b[30m";
        inline constexpr char red[]          = "\x1b[31m";
        inline constexpr char green[]        = "\x1b[32m";
        inline constexpr char yellow[]       = "\x1b[33m";
        inline constexpr char blue[]         = "\x1b[34m";
        inline constexpr char magenta[]      = "\x1b[35m";
        inline constexpr char cyan[]         = "\x1b[36m";
        inline constexpr char white[]        = "\x1b[37m";

        inline constexpr char lightBlack[]   = "\x1b[90m";
        inline constexpr char lightRed[]     = "\x1b[91m";
        inline constexpr char lightGreen[]   = "\x1b[92m";
        inline constexpr char lightYellow[]  = "\x1b[93m";
        inline constexpr char lightBlue[]    = "\x1b[94m";
        inline constexpr char lightMagenta[] = "\x1b[95m";
        inline constexpr char lightCyan[]    = "\x1b[96m";
        inline constexpr char lightWhite[]   = "\x1b[97m";
    }

    namespace back {
        inline constexpr char red[]          = "\x1b[41m";
        inline constexpr char green[]        = "\x1b[42m";
        inline constexpr char yellow[]       = "\x1b[43m";
        inline constexpr char blue[]         = "\x1b[44m";
        inline constexpr char magenta[]      = "\x1b[45m";
        inline constexpr char cyan[]         = "\x1b[46m";
        inline constexpr char white[]        = "\x1b[47m";

        inline constexpr char lightBlack[]   = "\x1b[100m";
        inline constexpr char lightRed[]     = "\x1b[101m";
        inline constexpr char lightGreen[]   = "\x1b[102m";
        inline constexpr char lightYellow[]  = "\x1b[103m";
        inline constexpr char lightBlue[]    = "\x1b[104m";
        inline constexpr char lightMagenta[] = "\x1b[105m";
        inline constexpr char lightCyan[]    = "\x1b[106m";
        inline constexpr char lightWhite[]   = "\x1b[107m";
    }
}

#endif

