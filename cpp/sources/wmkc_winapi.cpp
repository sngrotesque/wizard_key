#include <wmkc_winapi.hpp>

struct wmkcWinapi::widthHeight wmkcWinapi::getScreenResolution()
{
    wmkcWinapi::widthHeight size;
    size.width = GetSystemMetrics(SM_CXSCREEN);
    size.height = GetSystemMetrics(SM_CYSCREEN);
    if(!size.width || !size.height) {
        wmkcErr_exception(wmkcErr_Err32, "wmkcWinapi::getScreenResolution",
            "GetSystemMetrics function returned an error code when called.");
    }
}

struct wmkcWinapi::widthHeight wmkcWinapi::getCursorPos()
{
    wmkcWinapi::widthHeight point;
    POINT _point_tmp = {0};
    if(!GetCursorPos(&_point_tmp)) {
        wmkcErr_exception(GetLastError(), "wmkcWinapi::getCursorPos",
            "GetCursorPos function returned an error code when called.");
    }
    point.width = _point_tmp.x;
    point.height = _point_tmp.y;
}

std::string wmkcWinapi::getUserName(wmkcChar format = 'A')
{
    if(format == 'A') {
        wmkcChar tmp_buffer[512] = {0};
        DWORD tmp_buffer_size = sizeof(tmp_buffer);
        GetUserNameA(tmp_buffer, &tmp_buffer_size);
        return std::string(tmp_buffer);
    } else if(format == 'W') {
        WCHAR tmp_buffer[512] = {0};
        DWORD tmp_buffer_size = sizeof(tmp_buffer) >> 1;
        GetUserNameW(tmp_buffer, &tmp_buffer_size);
        return std::string((wmkcChar *)tmp_buffer, tmp_buffer_size);
    }
}

wmkcVoid wmkcWinapi::setCursorPos(wmkc_u32 x, wmkc_u32 y)
{
    if(!SetCursorPos(x, y)) {
        wmkcErr_exception(wmkcErr_ErrSysFunc, "wmkcWinapi::setCursorPos",
            "SetCursorPos function returned an error code when called.");
    }
}

/***
 *  还未写完。setDesktopWallpaper与opacityStartMenu成员函数并未编写
*/
