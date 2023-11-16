#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_WINAPI
#define WMKC_WINAPI
#include <wmkc_error.h>
#ifdef WMKC_PLATFORM_WINOS
#include <Windows.h>

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
WMKC_OF((wmkc_u32 *width, wmkc_u32 *height));

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
WMKC_OF((LPSTR name, LPDWORD size));

/**
 * @brief 设置鼠标指针的坐标
 * @authors SN-Grotesque
 * @note 无
 * @param x 为从左至右的横坐标。
 * @param y 为从上至下的纵坐标。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcWinapi_setCursorPos WMKC_OPEN_API
WMKC_OF((wmkc_u32 x, wmkc_u32 y));

/**
 * @brief 设置系统桌面壁纸
 * @authors SN-Grotesque
 * @note 立即更新并且更新用户配置
 * @param path 指针，指向图片文件路径的字符串地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcWinapi_setDesktopWallpaper WMKC_OPEN_API
WMKC_OF((LPWSTR path));

/**
 * @brief 设置开始菜单透明度
 * @authors SN-Grotesque
 * @note 此函数目前只能将开始菜单设置为透明或不透明，不包含任何效果（如亚克力）
 * @param bAlpha 开始菜单的不透明度
 * @return wmkcErr对象code为非0代表出错，需检查。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcWinapi_opacityStartMenu WMKC_OPEN_API
WMKC_OF((BYTE bAlpha));

#endif /* WMKC_PLATFORM_WINOS */
#endif /* WMKC_WINAPI */
#endif /* WMKC_SUPPORT */
