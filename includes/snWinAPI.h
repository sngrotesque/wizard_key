#if defined(_WIN32) || defined(_WIN64) || defined(MS_WINDOWS)
#ifndef __SN_WINAPI__
#define __SN_WINAPI__
#include <snConf.h>
#include <Windows.h>

#define SN_WINAPI_TEXT_W(x) L ## x
#define SN_WINAPI_TEXT_A(x) x




#endif // #ifndef __SN_WINAPI__
#endif // #if defined(_WIN32) || defined(_WIN64) || defined(MS_WINDOWS)
