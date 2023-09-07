#include <wmkc_winapi.h>

/**
 * @brief 获取屏幕的宽度和高度
 * @authors SN-Grotesque
 * @note 后续可能需要优化一下，因为此函数只能获取主显示器的宽高。
 * @param width 指针，指向wmkc_u32变量的地址。
 * @param height 指针，指向wmkc_u32变量的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcWinapi_getScreenWidthHeight WMKC_OPEN_API
WMKC_OF((wmkc_u32 *width, wmkc_u32 *height))
{
    wmkcErr_obj error;
    if(!width || !height) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcWinapi_getScreenWidthHeight",
            "width or height is NULL.");
    }

    *width = GetSystemMetrics(SM_CXSCREEN);
    *height =  GetSystemMetrics(SM_CYSCREEN);

    if(!(*width) || !(*height)) {
        wmkcErr_return(error, wmkcErr_ErrSysFunc, "wmkcWinapi_getScreenWidthHeight",
            "GetSystemMetrics function returned an error code when called.");
    }

    wmkcErr_return(error, wmkcErr_OK, "wmkcWinapi_getScreenWidthHeight", "OK.");
}

/**
 * @brief 获取系统当前的用户名
 * @authors SN-Grotesque
 * @note 此函数或许得优化一下，因为目前不接受WCHAR类型的数组。
 * @param name 指针，指向名字的缓冲区的地址。
 * @param size 指针，指向名字缓冲区长度类型的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcWinapi_getUserName WMKC_OPEN_API
WMKC_OF((LPSTR name, LPDWORD size))
{
    wmkcErr_obj error;
    if(!name) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcWinapi_getUserName",
            "name is NULL.");
    }

    if(!GetUserNameA(name, size)) {
        wmkcErr_return(error, wmkcErr_ErrSysFunc, "wmkcWinapi_getUserName",
            "GetUserNameA function returned an error code when called.");
    }

    wmkcErr_return(error, wmkcErr_OK, "wmkcWinapi_getUserName", "OK.");
}

/**
 * @brief 设置鼠标的坐标
 * @authors SN-Grotesque
 * @note 无
 * @param x 为从左至右的横坐标。
 * @param y 为从上至下的纵坐标。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcWinapi_setCursorPos WMKC_OPEN_API
WMKC_OF((wmkc_u32 x, wmkc_u32 y))
{
    wmkcErr_obj error;
    if(!SetCursorPos(x, y)) {
        wmkcErr_return(error, wmkcErr_ErrSysFunc, "wmkcWinapi_setCursorPos",
            "SetCursorPos function returned an error code when called.");
    }

    wmkcErr_return(error, wmkcErr_OK, "wmkcWinapi_setCursorPos", "OK.");
}

/**
 * @brief 设置系统桌面壁纸
 * @authors SN-Grotesque
 * @note 立即更新并且更新用户配置
 * @param path 指针，指向图片文件路径的字符串地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcWinapi_setDesktopWallpaper WMKC_OPEN_API
WMKC_OF((LPWSTR path))
{
    wmkcErr_obj error;
    if(!path) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcWinapi_getUserName",
            "path is NULL.");
    }
    if(!SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, path, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE)) {
        wmkcErr_return(error, wmkcErr_ErrSysFunc, "wmkcWinapi_setDesktopWallpaper",
            "SystemParametersInfoW function returned an error code when called.");
    }

    wmkcErr_return(error, wmkcErr_OK, "wmkcWinapi_setDesktopWallpaper", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcWinapi_opacityStartMenu WMKC_OPEN_API
WMKC_OF((BYTE bAlpha))
{
    wmkcErr_obj error;
    HWND handle = FindWindowW(L"Windows.UI.Core.CoreWindow", L"启动");
    if(!handle) {
        wmkcErr_return(error, wmkcErr_ErrSysFunc, "wmkcWinapi_opacityStartMenu",
            "The handle was not found. Please check the window name.");
    }

    if(!SetWindowLongW(handle, GWL_EXSTYLE, GetWindowLongW(handle, GWL_EXSTYLE) | WS_EX_LAYERED)) {
        wmkcErr_return(error, wmkcErr_ErrSysFunc, "wmkcWinapi_opacityStartMenu",
            "SetWindowLongW function returned an error code when called.");
    }
    if(!SetLayeredWindowAttributes(handle, 0, bAlpha, LWA_ALPHA)) {
        wmkcErr_return(error, wmkcErr_ErrSysFunc, "wmkcWinapi_opacityStartMenu",
            "SetLayeredWindowAttributes function returned an error code when called.");
    }

    wmkcErr_return(error, wmkcErr_OK, "wmkcWinapi_opacityStartMenu", "OK.");
}
