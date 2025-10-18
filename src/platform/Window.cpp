#include "Window.h"
#include "../core/Log.h"

#include <sstream>
#include <string>

// Forward declaration de ImGui Win32 handler (v1.3.0 - H2.3)
// Esta función está implementada en la biblioteca ImGui que linkamos
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Platform {

Window::Window(HINSTANCE hInstance, const wchar_t* title, int width, int height)
    : hInstance_(hInstance)
{
    const wchar_t CLASS_NAME[] = L"ImagineStudioWindowClass";

    // Use the module HINSTANCE for registration/creation to avoid mismatches
    HINSTANCE regInstance = hInstance_ ? hInstance_ : GetModuleHandle(NULL);

    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc   = WndProcStatic;
    wc.hInstance     = regInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);

    // Register class and report errors if any (Unicode)
    ATOM a = RegisterClassExW(&wc);
    if (a == 0)
    {
        DWORD err = GetLastError();
        std::ostringstream ss; ss << "Window::Window - RegisterClassExW failed, GetLastError=" << err; CORE_LOG_ERROR(ss.str());
    }
    else
    {
        std::ostringstream ss; ss << "Window::Window - RegisterClassExW succeeded, atom=" << a << " regInstance=" << reinterpret_cast<void*>(regInstance); CORE_LOG_INFO(ss.str());
        // Verify class info
        WNDCLASSEXW existing = {};
        existing.cbSize = sizeof(existing);
        if (GetClassInfoExW(regInstance, CLASS_NAME, &existing) || GetClassInfoExW(NULL, CLASS_NAME, &existing))
        {
            CORE_LOG_INFO("Window::Window - GetClassInfoExW succeeded for registered class");
        }
        else
        {
            std::ostringstream ss2; ss2 << "Window::Window - GetClassInfoExW failed, GetLastError=" << GetLastError(); CORE_LOG_ERROR(ss2.str());
        }
    }

    LPCWSTR classParam = CLASS_NAME;
    if (a != 0) classParam = MAKEINTATOM(a);

    hwnd_ = CreateWindowExW(
        0,
        classParam,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL,
        NULL,
        regInstance,
        this
    );
    if (!hwnd_)
    {
        DWORD err = GetLastError();
        std::ostringstream ss; ss << "Window::Window - CreateWindowEx failed, GetLastError=" << err; CORE_LOG_ERROR(ss.str());

        // Fallback test: try creating a simple window using a built-in class
        // (STATIC) to determine whether custom class registration is the issue.
        hwnd_ = CreateWindowExW(0, L"STATIC", title, WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT, width, height,
                                NULL, NULL, regInstance, this);
        if (hwnd_)
        {
            std::ostringstream ss2; ss2 << "Window::Window - fallback using WC_OVERLAPPEDWINDOW succeeded, HWND=" << reinterpret_cast<void*>(hwnd_); CORE_LOG_INFO(ss2.str());
            ShowWindow(hwnd_, SW_SHOW);
        }
        else
        {
            DWORD err2 = GetLastError();
            std::ostringstream ss3; ss3 << "Window::Window - fallback CreateWindowEx failed, GetLastError=" << err2; CORE_LOG_ERROR(ss3.str());
            // Show an explicit message to aid debugging with readable text
            LPWSTR msgBuf = nullptr;
            FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, err2, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&msgBuf), 0, NULL);
            if (msgBuf)
            {
                MessageBoxW(NULL, msgBuf, L"Imagine Studio - CreateWindowEx Error", MB_OK | MB_ICONERROR);
                LocalFree(msgBuf);
            }
            else
            {
                MessageBoxW(NULL, L"CreateWindowEx failed (unknown error)", L"Imagine Studio - CreateWindowEx Error", MB_OK | MB_ICONERROR);
            }
        }
    }
    else
    {
        std::ostringstream ss; ss << "Window::Window - created HWND=" << reinterpret_cast<void*>(hwnd_); CORE_LOG_INFO(ss.str());
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
    // ? CRÍTICO: Pasar eventos a ImGui PRIMERO (v1.3.0 - H2.3)
    // Llamar SIEMPRE al handler de ImGui para que capture eventos internamente
    // NO retornar inmediatamente - ImGui procesa eventos pero retorna 0 en mayoría de casos
    // El handler actualiza io.WantCaptureMouse, io.WantCaptureKeyboard internamente
    ImGui_ImplWin32_WndProcHandler(hwnd_, message, wParam, lParam);
    
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd_, &ps);
        RECT rc;
        GetClientRect(hwnd_, &rc);
        // Fill background using system color
        HBRUSH brush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
        FillRect(hdc, &rc, brush);
        DeleteObject(brush);
        // Draw a simple test string to verify UI is rendering
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0,0,0));
        DrawTextW(hdc, L"Hola Mundo", -1, &rc, DT_SINGLELINE | DT_TOP | DT_LEFT);
        EndPaint(hwnd_, &ps);
        return 0;
    }
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
