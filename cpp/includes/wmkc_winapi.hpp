#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_WINAPI
#define WMKC_CPP_WINAPI
#include <wmkc_exception.hpp>
#include <Windows.h>

class wmkcWinapi {
    public:
        struct widthHeight {
            wmkc_u32 width;
            wmkc_u32 height;
        };
        struct wmkcWinapi::widthHeight getScreenResolution();
        struct wmkcWinapi::widthHeight getCursorPos();
        std::string getUserName(wmkcChar format = 'A');
        wmkcVoid setCursorPos(wmkc_u32 x, wmkc_u32 y);
        wmkcVoid setDesktopWallpaper(std::string path);
        wmkcVoid opacityStartMenu(BYTE alpha);
};

#endif
#endif
