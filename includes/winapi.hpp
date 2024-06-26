#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_WINAPI
#define WMKC_CPP_WINAPI
#include <config/exception.hpp>
#include <Windows.h>

namespace wmkc {
    namespace winapi {
        struct widthHeight {
            wU32 width;
            wU32 height;
        };
        struct widthHeight get_screen_resolution();
        struct widthHeight get_cursor_pos();
        std::string get_username(char format = 'A');
        void set_cursor_pos(wU32 x, wU32 y);
        void set_desktop_wallpaper(std::string path);
        void opacity_startmenu(BYTE alpha);
    };
}

#endif
#endif
