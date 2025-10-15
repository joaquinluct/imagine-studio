// Application entry using Platform::Window
#include <windows.h>
#include "platform/Window.h"
#include "platform/Input.h"
#include "renderer/DX12Renderer.h"
#include "ui/SimpleUI.h"
#include "assets/AssetManager.h"
#include "core/Log.h"

static int RunApp(HINSTANCE hInstance)
{
    Platform::Window window(hInstance, L"Imagine Studio - Window", 800, 600);

    // Simple message loop
    Platform::InputManager input;
    Renderer::DX12Renderer renderer;
    UI::SimpleUI ui;

    renderer.Initialize();
    ui.Initialize(window.GetHWND());

    Assets::AssetManager assets;
    assets.Initialize();
    assets.LoadAsync("example_asset.dat", [](const std::string& p){ CORE_LOG_INFO(std::string("Callback: asset loaded: ") + p); });

    while (window.ProcessMessages())
    {
        input.Update(window.GetHWND());

        // Example: if Escape pressed, exit
        if (input.IsKeyDown(VK_ESCAPE))
            break;

        renderer.RenderFrame();
        ui.Draw();
        // TODO: update and render calls will go here
    }

    assets.Shutdown();
    ui.Shutdown();
    renderer.Shutdown();

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
