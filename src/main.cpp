// Application entry using Platform::Window
#include <windows.h>
#include "platform/Window.h"
#include "core/Log.h"

static int RunApp(HINSTANCE hInstance)
{
    Platform::Window window(hInstance, L"Imagine Studio - Window", 800, 600);

    // Simple message loop
    while (window.ProcessMessages())
    {
        // TODO: update and render calls will go here
        // For now, just idle until WM_QUIT
    }

    CORE_LOG_INFO("Application exiting");

    return 0;
}

// Console entry
int main()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    return RunApp(hInstance);
}

// Windows GUI entry
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    return RunApp(hInstance);
}
