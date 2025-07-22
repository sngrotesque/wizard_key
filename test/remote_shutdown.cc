#include <network/WukSocket.hh>

#include <network/WukSocket.cc>

#include <iostream>
#include <windows.h>

void power_off()
{
    // 获取关机令牌
    HANDLE hToken{nullptr};
    TOKEN_PRIVILEGES tkp;

    DWORD DesiredAccess = TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY;

    if (!OpenProcessToken(GetCurrentProcess(), DesiredAccess, &hToken)) {
        throw wuk::Exception{GetLastError(), "enable_shutdown_privilege", "获取进程令牌失败！"};
    }

    LookupPrivilegeValueA(nullptr, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, nullptr, 0)) {
        throw wuk::Exception(GetLastError(), "enable_shutdown_privilege", "调整令牌特权失败！");
    }
    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
        throw wuk::Exception(GetLastError(), "enable_shutdown_privilege", "未授予所有请求的特权！");
    }

    CloseHandle(hToken);

    // 执行关机命令
    if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_POWEROFF | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER)) {
        throw wuk::Exception(GetLastError(), "power_off", "关闭系统失败！");
    }
}

void set_run_timer(DWORD seconds)
{
    LARGE_INTEGER liDueTime;
    liDueTime.QuadPart = -static_cast<LONGLONG>(seconds * 10000000);  // 单位是100纳秒，负值表示相对时间

    HANDLE hTimer = CreateWaitableTimer(NULL, TRUE, NULL);

    if (hTimer == NULL) {
        throw wuk::Exception(GetLastError(), "set_run_timer", "创建定时器失败！");
    }

    if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, FALSE)) {
        CloseHandle(hTimer);
        throw wuk::Exception(GetLastError(), "set_run_timer", "设置定时器失败！");
    }

    std::cout << "System will shutdown in " << seconds << " seconds..." << std::endl;
    WaitForSingleObject(hTimer, INFINITE);

    CloseHandle(hTimer);
}

void relaunch_as_admin()
{
    char sz_path[MAX_PATH];
    SHELLEXECUTEINFOA sei = {sizeof(SHELLEXECUTEINFOA)};

    sei.lpVerb = "runas";
    sei.lpFile = sz_path;
    sei.hwnd = nullptr;
    sei.nShow = SW_NORMAL;

    if (!GetModuleFileNameA(nullptr, sz_path, ARRAYSIZE(sz_path))) {
        throw wuk::Exception(GetLastError(), "relaunch_as_admin", "获取当前文件的路径失败！");
    }

    if (ShellExecuteEx(&sei)) {
        ExitProcess(0);
    } else {
        DWORD dw_error = GetLastError();
        if (dw_error == ERROR_CANCELLED) {
            throw wuk::Exception(dw_error, "relaunch_as_admin", "用户拒绝了权限提升的请求。");
        }
        throw wuk::Exception(dw_error, "relaunch_as_admin", "其他错误！");
    }
}

void locale_check()
{
    if (GetConsoleOutputCP() == CP_UTF8) {
        return;
    }

    if (!SetConsoleOutputCP(CP_UTF8)) {
        throw wuk::Exception(GetLastError(), "locale_check", "Unable to set chcp to UTF-8.");
    }
}

void remote_shutdown_listen(std::string laddr, wU16 lport)
{
    wuk::net::Socket fd{AF_INET, SOCK_DGRAM, IPPROTO_UDP};

    // fd.sendto("test done.", );

}

int main()
{
    try {
        remote_shutdown_listen("0.0.0.0", 39915);
    } catch (wuk::Exception &e) {
        std::cout << e.what() << std::endl;
    }

}
