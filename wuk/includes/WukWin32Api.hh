#pragma once
#include <config/WukConfig.hh>

#ifdef WUK_PLATFORM_WINOS
#if WUK_SUPPORT
#include <config/WukException.hh>
#include <WukMemory.hh>

#include <Windows.h>

namespace wuk {
    class WinApi {
    private:
        char err_msg[256];
        wuk::Error err_code;
    
    private:
        void get_error() noexcept;

    public:
        WinApi() = default;

        POINT get_cursor_pos();
        void set_cursor_pos(wI32 x, wI32 y) const noexcept;
        void set_cursor_pos(POINT pos) const noexcept;
        void set_cursor_pos(MOUSEINPUT m) const noexcept;

        template <typename T>
        T get_username();

        template <typename T>
        void set_wallpaper(T path) const noexcept;

        template <typename T>
        void set_window_opacity(T class_name, T window_name, BYTE bAlpha);
    };
}

#endif
#endif
