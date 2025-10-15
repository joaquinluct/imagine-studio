#pragma once
#include <windows.h>

namespace Platform {

class Window {
public:
    Window(HINSTANCE hInstance, const wchar_t* title, int width, int height);
    ~Window();

    // Process window messages. Returns false when the application should quit.
    bool ProcessMessages();

    HWND GetHWND() const { return hwnd_; }

private:
    HWND hwnd_ = NULL;
    HINSTANCE hInstance_ = NULL;
    static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);
};

} // namespace Platform
