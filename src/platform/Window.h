#pragma once
#include <windows.h>
#include <map>

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
    
    // ?? BUG-002 Intento #6: Map est�tico para asociar HWND con Window*
    // Esto evita problemas con GWLP_USERDATA en WM_NCCREATE
    static std::map<HWND, Window*> s_windowMap;
    
    static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);
};

} // namespace Platform
