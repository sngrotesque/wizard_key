#include <wmkc_winapi.h>

/**
 * @brief 获取屏幕的宽度和高度
 * @authors SN-Grotesque
 * @note 后续可能需要优化一下，因为此函数只能获取主显示器的宽高。
 * @param width 这是一个指针，指向wmkc_u32变量的地址。
 * @param height 这是一个指针，指向wmkc_u32变量的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcWinapi_getScreenWidthHeight WMKC_OPEN_API
WMKC_OF((wmkc_u32 *width, wmkc_u32 *height))
{
    wmkcErr_obj error;
    if(!width || !height) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcWinapi_getScreenWidthHeight: width or height is NULL.");
    }

    *width = GetSystemMetrics(SM_CXSCREEN);
    *height =  GetSystemMetrics(SM_CYSCREEN);

    if(!(*width) || !(*height)) {
        wmkcErr_return(error, wmkcErr_ErrSysFunc,
            "wmkcWinapi_getScreenWidthHeight: Error calling GetSystemMetrics function.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 获取系统当前的用户名
 * @authors SN-Grotesque
 * @note 此函数或许得优化一下，因为目前不接受WCHAR类型的数组。
 * @param name 这是一个指针，指向名字的缓冲区的地址。
 * @param size 这是一个指针，指向名字缓冲区长度类型的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcWinapi_getUserName WMKC_OPEN_API
WMKC_OF((LPSTR name, LPDWORD size))
{
    wmkcErr_obj error;
    if(!name) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcWinapi_getUserName: name is NULL.");
    }

    if(!GetUserNameA(name, size)) {
        wmkcErr_return(error, wmkcErr_ErrSysFunc,
            "wmkcWinapi_getUserName: Error calling GetUserNameW function.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
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
        wmkcErr_return(error, wmkcErr_ErrSysFunc,
            "wmkcWinapi_setCursorPos: Error calling SetCursorPos function.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 设置系统桌面壁纸
 * @authors SN-Grotesque
 * @note 立即更新并且更新用户配置
 * @param path 这是一个指针，指向图片文件路径的字符串地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcWinapi_setDesktopWallpaper WMKC_OPEN_API
WMKC_OF((LPWSTR path))
{
    wmkcErr_obj error;
    if(!path) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcWinapi_getUserName: path is NULL.");
    }
    if(!SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, path, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE)) {
        wmkcErr_return(error, wmkcErr_ErrSysFunc,
            "wmkcWinapi_setDesktopWallpaper: Error calling SystemParametersInfoW function.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
