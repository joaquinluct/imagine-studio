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

    // ?? BUG-002 Intento #5: Usar SIEMPRE GetModuleHandle(NULL) para consistencia
    // Esto asegura que usamos el mismo HINSTANCE en registro y creación
    HINSTANCE moduleInstance = GetModuleHandle(NULL);

    // ? Verificar si la clase ya está registrada
    WNDCLASSEXW existingClass = {};
    existingClass.cbSize = sizeof(existingClass);
    BOOL classExists = GetClassInfoExW(moduleInstance, CLASS_NAME, &existingClass);
    
    if (classExists)
    {
        std::ostringstream ss; 
        ss << "Window::Window - Class already registered with HINSTANCE=" 
           << reinterpret_cast<void*>(existingClass.hInstance);
        CORE_LOG_INFO(ss.str());
    }
    else
    {
        // ?? Registrar clase solo si no existe
        WNDCLASSEXW wc = {};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.lpfnWndProc   = WndProcStatic;
        wc.hInstance     = moduleInstance;
        wc.lpszClassName = CLASS_NAME;
        wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);

        ATOM atom = RegisterClassExW(&wc);
        if (atom == 0)
        {
            DWORD err = GetLastError();
            std::ostringstream ss; 
            ss << "Window::Window - RegisterClassExW FAILED, GetLastError=" << err;
            CORE_LOG_ERROR(ss.str());
            
            // ? CRÍTICO: Abortar si no podemos registrar la clase
            MessageBoxW(NULL, L"Failed to register window class. Cannot continue.", 
                       L"Imagine Studio - Fatal Error", MB_OK | MB_ICONERROR);
            return;
        }
        
        std::ostringstream ss; 
        ss << "Window::Window - RegisterClassExW succeeded, atom=" << atom 
           << " HINSTANCE=" << reinterpret_cast<void*>(moduleInstance);
        CORE_LOG_INFO(ss.str());
    }

    // ?? Crear ventana usando el mismo HINSTANCE que usamos para registro
    hwnd_ = CreateWindowExW(
        0,
        CLASS_NAME,  // ?? Usar nombre de clase directamente (no atom)
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL,
        NULL,
        moduleInstance,  // ? Mismo HINSTANCE que registro
        this
    );
    
    if (!hwnd_)
    {
        DWORD err = GetLastError();
        std::ostringstream ss; 
        ss << "Window::Window - CreateWindowExW FAILED, GetLastError=" << err;
        CORE_LOG_ERROR(ss.str());

        // ?? BUG-002 Intento #5: ELIMINAR fallback a clase STATIC
        // El fallback enmascara el problema real y crea ventanas no-interactivas
        
        // ? Mostrar error detallado y abortar
        LPWSTR msgBuf = nullptr;
        FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
            reinterpret_cast<LPWSTR>(&msgBuf), 0, NULL
        );
        
        std::wstring errorMsg = L"CreateWindowExW failed with error " + std::to_wstring(err) + L":\n";
        if (msgBuf)
        {
            errorMsg += msgBuf;
            LocalFree(msgBuf);
        }
        else
        {
            errorMsg += L"(Unknown error)";
        }
        
        MessageBoxW(NULL, errorMsg.c_str(), 
                   L"Imagine Studio - Fatal Error", MB_OK | MB_ICONERROR);
        return;
    }
    
    std::ostringstream ss; 
    ss << "Window::Window - CreateWindowExW succeeded, HWND=" << reinterpret_cast<void*>(hwnd_);
    CORE_LOG_INFO(ss.str());
    
    ShowWindow(hwnd_, SW_SHOW);
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
    // ?? DEBUG: Log mouse button events (BUG-002 Intento #4)
    if (message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_MBUTTONDOWN)
    {
        std::ostringstream ss;
        ss << "[WndProc] Mouse button DOWN message received: msg=" << message 
           << " (WM_LBUTTONDOWN=" << WM_LBUTTONDOWN 
           << "), wParam=" << wParam 
           << ", lParam=" << lParam;
        CORE_LOG_INFO(ss.str());
    }
    
    if (message == WM_LBUTTONUP || message == WM_RBUTTONUP || message == WM_MBUTTONUP)
    {
        std::ostringstream ss;
        ss << "[WndProc] Mouse button UP message received: msg=" << message 
           << " (WM_LBUTTONUP=" << WM_LBUTTONUP 
           << "), wParam=" << wParam 
           << ", lParam=" << lParam;
        CORE_LOG_INFO(ss.str());
    }
    
    // ? CRÍTICO: Pasar eventos a ImGui PRIMERO y respetar su retorno (v1.3.0 - H2.3)
    // Si ImGui captura el evento (retorna != 0), NO debemos procesarlo nosotros
    // Esto es ESENCIAL para que los clicks de ratón funcionen en la UI
    LRESULT imgui_result = ImGui_ImplWin32_WndProcHandler(hwnd_, message, wParam, lParam);
    
    // ?? DEBUG: Log handler result (BUG-002 Intento #4)
    if (message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_MBUTTONDOWN ||
        message == WM_LBUTTONUP || message == WM_RBUTTONUP || message == WM_MBUTTONUP)
    {
        std::ostringstream ss;
        ss << "[WndProc] ImGui_ImplWin32_WndProcHandler returned: " << imgui_result;
        CORE_LOG_INFO(ss.str());
        
        if (imgui_result)
        {
            CORE_LOG_INFO("[WndProc] ImGui captured event, returning early");
        }
        else
        {
            CORE_LOG_INFO("[WndProc] ImGui did NOT capture event, continuing to DefWindowProc");
        }
    }
    
    if (imgui_result)
        return true;
    
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
