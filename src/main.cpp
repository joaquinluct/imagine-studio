// Application entry using Platform::Window
#include "assets/AssetManager.h"
#include "core/Log.h"
#include "jobs/TaskGraph.h"
#include "jobs/ThreadPool.h"
#include "platform/Input.h"
#include "platform/Window.h"
#include "renderer/DX12Renderer.h"
#include "ui/SimpleUI.h"

#include <windows.h>
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

    // Demonstration: async load an asset and signal a fence when complete
    Renderer::Fence assetFence;
    assets.LoadAsync("example_asset.dat", [](const std::string& p){ CORE_LOG_INFO(std::string("Callback: asset loaded: ") + p); }, &assetFence);

    // Wait for the asset to be loaded (synchronization test)
    CORE_LOG_INFO("Waiting for async asset to load...");
    assetFence.WaitForValue(1);
    CORE_LOG_INFO("Async asset load signaled");

    // 7.01 TaskGraph basic test: create simple dependency A,B -> C
    {
        CORE_LOG_INFO("TaskGraph test: start");
        Jobs::ThreadPool tgPool(3);
        Jobs::TaskGraph tg;
        int tA = tg.AddTask([](){ CORE_LOG_INFO("TaskGraph: Task A executed"); });
        int tB = tg.AddTask([](){ CORE_LOG_INFO("TaskGraph: Task B executed"); });
        int tC = tg.AddTask([](){ CORE_LOG_INFO("TaskGraph: Task C executed (after A,B)"); });
        tg.AddDependency(tA, tC);
        tg.AddDependency(tB, tC);
        tg.Execute(&tgPool);
        CORE_LOG_INFO("TaskGraph test: completed");
        // tgPool will be shutdown in destructor
    }

    // Streaming test: enqueue multiple loads with different priorities and cancel one
    CORE_LOG_INFO("Starting streaming priority/cancelation test");
    auto hHigh = assets.LoadAsync("asset_high.dat", [](const std::string& p){ CORE_LOG_INFO(std::string("Callback: asset_high: ") + p); }, nullptr, Assets::AssetManager::Priority::High);
    auto hLow = assets.LoadAsync("asset_low.dat", [](const std::string& p){ CORE_LOG_INFO(std::string("Callback: asset_low: ") + p); }, nullptr, Assets::AssetManager::Priority::Low);
    auto hNormal = assets.LoadAsync("asset_normal.dat", [](const std::string& p){ CORE_LOG_INFO(std::string("Callback: asset_normal: ") + p); }, nullptr, Assets::AssetManager::Priority::Normal);

    // Cancel the low priority load to test cancellation
    bool cancelled = assets.CancelLoad(hLow);
    CORE_LOG_INFO(std::string("Requested cancel for asset_low: ") + (cancelled ? "true" : "false"));

    while (window.ProcessMessages())
    {
        input.Update(window.GetHWND());

        // Example: if Escape pressed, exit
        if (input.IsKeyDown(VK_ESCAPE))
            break;

        // Poll for loaded assets and notify renderer
        std::string loadedPath;
        while (assets.PopLoaded(loadedPath))
        {
            renderer.OnAssetLoaded(loadedPath);
        }

        renderer.RenderFrame();
        ui.Draw();
        // TODO: update and render calls will go here
    }

    assets.Shutdown();
    assets.DumpMetrics();
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
// Suppress SAL annotation mismatch for WinMain wrapper calling RunApp
#pragma warning(push)
#pragma warning(disable:28251)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    return RunApp(hInstance);
}
#pragma warning(pop)
