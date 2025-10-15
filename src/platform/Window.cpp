#include "Window.h"

namespace Platform {

Window::Window(HINSTANCE hInstance, const wchar_t* title, int width, int height)
    : hInstance_(hInstance)
{
    const wchar_t CLASS_NAME[] = L"ImagineStudioWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc   = WndProcStatic;
    wc.hInstance     = hInstance_;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    hwnd_ = CreateWindowEx(
        0,
        CLASS_NAME,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL,
        NULL,
        hInstance_,
        this
    );

    if (hwnd_)
    {
        ShowWindow(hwnd_, SW_SHOW);
    }
}

Window::~Window()
{
    if (hwnd_)
        DestroyWindow(hwnd_);
}

LRESULT CALLBACK Window::WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_NCCREATE)
    {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        Window* win = reinterpret_cast<Window*>(cs->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(win));
    }

    Window* win = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (win)
        return win->WndProc(message, wParam, lParam);

    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT Window::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd_, message, wParam, lParam);
    }
}

bool Window::ProcessMessages()
{
    MSG msg = {};
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return false;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}

} // namespace Platform
