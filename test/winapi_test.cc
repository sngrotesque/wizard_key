// python make.py test\winapi_test.cc -DWUK_EXPORTS -mwindows -municode
#include <config/WukConfig.hh>
#ifdef WUK_PLATFORM_WINOS
#include <WukRandom.cc>
#include <WukTime.cc>
#include <windows.h>
#include <thread>

// 窗口过程回调函数
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                DestroyWindow(hwnd);
            }
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// 创建自定义消息框
void CreateCustomMessageBox(HINSTANCE hInstance, LPCWSTR lpText, LPCWSTR lpCaption, int x, int y) {
    const WCHAR CLASS_NAME[] = L"CustomMessageBoxClass";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        lpCaption,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        x, y, 300, 150,
        nullptr, nullptr, hInstance, nullptr
    );

    if (hwnd == nullptr) {
        MessageBoxW(nullptr, L"Failed to create window.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    // 创建静态文本
    CreateWindowW(
        L"STATIC",
        lpText,
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        20, 20, 260, 20,
        hwnd,
        nullptr,
        hInstance,
        nullptr
    );

    // 创建 "确定" 按钮
    CreateWindowW(
        L"BUTTON",
        L"OK",
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        110, 70, 80, 30,
        hwnd,
        (HMENU)IDOK,
        hInstance,
        nullptr
    );

    // 进入消息循环
    MSG msg{};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// 在新线程中创建消息框
void CreateMessageBoxThread(HINSTANCE hInstance) {
    wuk::Random rad;
    int x = rad.randint(1, 2560);
    int y = rad.randint(1, 1440);
    CreateCustomMessageBox(hInstance, L"捅死你喵！", L"捅死你喵！", x, y);
}

void CreateRandomCursorPos()
{
    wuk::Random rad;
    SetCursorPos(rad.randint(1, 2560-1), rad.randint(1, 1440-1));
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    hPrevInstance = hPrevInstance;
    lpCmdLine = lpCmdLine;
    nCmdShow = nCmdShow;
    for (DWORD count = 0; count < 128; ++count) {
        std::thread(CreateMessageBoxThread, hInstance).detach(); // 创建独立的线程处理窗口
        std::thread(CreateRandomCursorPos).detach();
        Sleep(60); // 可选：添加延时以避免同时创建过多窗口
    }
    Sleep(1820);
    return 0;
}
#endif
