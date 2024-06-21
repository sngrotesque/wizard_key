#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_WINAPI
#define WMKC_CPP_WINAPI
#include <config/exception.hpp>
#include <Windows.h>

namespace wmkc {
    class LIBWMKC_API Winapi {
    public:
        struct widthHeight {
            wU32 width;
            wU32 height;
        };
        struct widthHeight getScreenResolution();
        struct widthHeight getCursorPos();
        std::string getUserName(char format = 'A');
        void setCursorPos(wU32 x, wU32 y);
        void setDesktopWallpaper(std::string path);
        void opacityStartMenu(BYTE alpha);
    };
}

#endif
#endif
