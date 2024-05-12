#include <config/wmkc.hpp>

// 后续看看情况是否将这些字符串改为std::string

#if WMKC_SUPPORT
#ifndef WMKC_COLOR
#define WMKC_COLOR

#define WMKC_COLOR_ALL_RESET         "\x1b[0m"  // 重置所有颜色

#define WMKC_COLOR_ALL_FLICKER       "\x1b[5m"  // 闪烁的字符

#define WMKC_COLOR_FORE_BLACK        "\x1b[30m" // 黑色（前景）
#define WMKC_COLOR_FORE_RED          "\x1b[31m" // 红色（前景）
#define WMKC_COLOR_FORE_GREEN        "\x1b[32m" // 绿色（前景）
#define WMKC_COLOR_FORE_YELLOW       "\x1b[33m" // 黄色（前景）
#define WMKC_COLOR_FORE_BLUE         "\x1b[34m" // 蓝色（前景）
#define WMKC_COLOR_FORE_MAGENTA      "\x1b[35m" // 紫色（前景）
#define WMKC_COLOR_FORE_CYAN         "\x1b[36m" // 青色（前景）
#define WMKC_COLOR_FORE_WHITE        "\x1b[37m" // 白色（前景）

#define WMKC_COLOR_FORE_LIGHTBLACK   "\x1b[90m" // 亮黑色（前景）
#define WMKC_COLOR_FORE_LIGHTRED     "\x1b[91m" // 亮红色（前景）
#define WMKC_COLOR_FORE_LIGHTGREEN   "\x1b[92m" // 亮绿色（前景）
#define WMKC_COLOR_FORE_LIGHTYELLOW  "\x1b[93m" // 亮黄色（前景）
#define WMKC_COLOR_FORE_LIGHTBLUE    "\x1b[94m" // 亮蓝色（前景）
#define WMKC_COLOR_FORE_LIGHTMAGENTA "\x1b[95m" // 亮紫色（前景）
#define WMKC_COLOR_FORE_LIGHTCYAN    "\x1b[96m" // 亮青色（前景）
#define WMKC_COLOR_FORE_LIGHTWHITE   "\x1b[97m" // 亮白色（前景）

#define WMKC_COLOR_BACK_RED          "\x1b[41m" // 红色（背景）
#define WMKC_COLOR_BACK_GREEN        "\x1b[42m" // 绿色（背景）
#define WMKC_COLOR_BACK_YELLOW       "\x1b[43m" // 黄色（背景）
#define WMKC_COLOR_BACK_BLUE         "\x1b[44m" // 蓝色（背景）
#define WMKC_COLOR_BACK_MAGENTA      "\x1b[45m" // 紫色（背景）
#define WMKC_COLOR_BACK_CYAN         "\x1b[46m" // 青色（背景）
#define WMKC_COLOR_BACK_WHITE        "\x1b[47m" // 白色（背景）

#define WMKC_COLOR_BACK_LIGHTBLACK   "\x1b[100m" // 亮黑色（背景）
#define WMKC_COLOR_BACK_LIGHTRED     "\x1b[101m" // 亮红色（背景）
#define WMKC_COLOR_BACK_LIGHTGREEN   "\x1b[102m" // 亮绿色（背景）
#define WMKC_COLOR_BACK_LIGHTYELLOW  "\x1b[103m" // 亮黄色（背景）
#define WMKC_COLOR_BACK_LIGHTBLUE    "\x1b[104m" // 亮蓝色（背景）
#define WMKC_COLOR_BACK_LIGHTMAGENTA "\x1b[105m" // 亮紫色（背景）
#define WMKC_COLOR_BACK_LIGHTCYAN    "\x1b[106m" // 亮青色（背景）
#define WMKC_COLOR_BACK_LIGHTWHITE   "\x1b[107m" // 亮白色（背景）

#define WMKC_COLOR_SET(color, text) color text WMKC_COLOR_ALL_RESET

#endif /* WMKC_COLOR */
#endif

