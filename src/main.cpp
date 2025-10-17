// Application entry using Platform::Window
#include "assets/AssetManager.h"
#include "core/Log.h"
#include "jobs/TaskGraph.h"
#include "jobs/ThreadPool.h"
#include "platform/Input.h"
#include "platform/Window.h"
#include "renderer/DX12Renderer.h"
#include "tools/Profiler.h"
#include "ui/SimpleUI.h"

#include <chrono>
#include <fstream>
#include <sstream>
#include <windows.h>
// For DPI helper functions
#include <VersionHelpers.h>
static int RunApp(HINSTANCE hInstance)
{
    // Make the process DPI aware so window coordinates and monitor bounds
    // are reported in the correct (physical) pixels. This helps ensure a
    // borderless fullscreen window covers the monitor correctly when the
    // system uses display scaling.
    // Prefer SetProcessDpiAwarenessContext if available on newer Windows,
    // but fall back to SetProcessDPIAware for compatibility.
    // Note: calling this after window creation has no effect, so do it here.
    if (IsWindows10OrGreater())
    {
        // Try per-monitor v2 if available (Windows 10+)
        typedef BOOL(WINAPI* SPDAC)(DPI_AWARENESS_CONTEXT);
        HMODULE hUser = GetModuleHandleW(L"user32.dll");
        if (hUser)
        {
            SPDAC spdac = reinterpret_cast<SPDAC>(GetProcAddress(hUser, "SetProcessDpiAwarenessContext"));
            if (spdac)
            {
                // DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 == -4
                spdac(reinterpret_cast<DPI_AWARENESS_CONTEXT>(-4));
            }
            else
            {
                // Fallback
                SetProcessDPIAware();
            }
        }
    }
    else
    {
        SetProcessDPIAware();
    }

    // Create the main application window (shown by default)
    Platform::Window window(hInstance, L"Imagine Studio - Window", 800, 600);

    // Ensure the window is shown. Then switch to a borderless window and
    // maximize it so it covers the whole monitor.
    HWND hwnd = window.GetHWND();
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    if (hwnd)
    {
        // Remove window decorations and set popup style
        LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
        style &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
        style |= WS_POPUP;
        SetWindowLongPtr(hwnd, GWL_STYLE, style);

        // Get monitor bounds and position the window explicitly to cover it.
        HMONITOR hm = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi = {};
        mi.cbSize = sizeof(mi);
        if (GetMonitorInfoW(hm, &mi))
        {
            int wx = mi.rcMonitor.left;
            int wy = mi.rcMonitor.top;
            int ww = mi.rcMonitor.right - mi.rcMonitor.left;
            int wh = mi.rcMonitor.bottom - mi.rcMonitor.top;
            // Apply the style change and resize to exact monitor area
            SetWindowPos(hwnd, HWND_TOP, wx, wy, ww, wh, SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
            SetForegroundWindow(hwnd);
        }
        else
        {
            // Fallback: ensure window is maximized
            ShowWindow(hwnd, SW_MAXIMIZE);
            SetForegroundWindow(hwnd);
        }
    }

    // (Removed diagnostic repositioning � the window was intentionally set
    // to borderless fullscreen above and repositioning here reverted it.)

    // Log HWND for diagnosis
    {
        std::ostringstream ss; ss << "Main window HWND=" << reinterpret_cast<void*>(window.GetHWND()); CORE_LOG_INFO(ss.str());
    }

    // Debug: window visibility logged above. No MessageBox so the main loop
    // remains responsive and the window is interactive during debugging.
    CORE_LOG_INFO(std::string("Render window should be visible (HWND=)") + std::to_string(reinterpret_cast<uintptr_t>(window.GetHWND())));

    // Simple message loop
    Platform::InputManager input;
    Renderer::DX12Renderer renderer;
    UI::SimpleUI ui;

    renderer.Initialize(hwnd);
    ui.Initialize(window.GetHWND());

    Assets::AssetManager assets;
    assets.Initialize();

    // Demonstration: async load an asset. Do not block the main thread.
    // The main loop will process loaded assets via assets.PopLoaded(...).
    assets.LoadAsync("example_asset.dat", [](const std::string& p){ CORE_LOG_INFO(std::string("Callback: asset loaded: ") + p); }, nullptr);

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
        auto frameStart = std::chrono::high_resolution_clock::now();
        input.Update(window.GetHWND());

        // Example: if Escape pressed, exit
        if (input.IsKeyDown(VK_ESCAPE))
            break;
        
        // H2.3: Toggle UI visibility with F1 key
        if (input.IsKeyPressed(VK_F1))
        {
            renderer.ToggleUI();
            CORE_LOG_INFO(std::string("UI visibility toggled: ") + (renderer.IsUIVisible() ? "visible" : "hidden"));
        }

        // Poll for loaded assets and notify renderer
        std::string loadedPath;
        while (assets.PopLoaded(loadedPath))
        {
            renderer.OnAssetLoaded(loadedPath);
        }

        renderer.RenderFrame();
        ui.Draw();
        ui.DrawOverlay();
        auto frameEnd = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(frameEnd - frameStart).count();
        Tools::Profiler::Instance().RecordFrame(ms);
        // TODO: update and render calls will go here
    }

    assets.Shutdown();
    assets.DumpMetrics();
    ui.Shutdown();
    renderer.Shutdown();

    CORE_LOG_INFO("Application exiting");

    return 0;
}

// Windows GUI entry (GUI-only executable)
// Suppress SAL annotation mismatch for WinMain wrapper calling RunApp
#pragma warning(push)
#pragma warning(disable:28251)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    try
    {
        return RunApp(hInstance);
    }
    catch (const std::exception& ex)
    {
        // Ensure logs directory exists relative to repository/workdir using Win32
        const char* dir = "build\\logs";
        CreateDirectoryA("build", NULL);
        CreateDirectoryA(dir, NULL);
        const char* out = "build\\logs\\crash.txt";
        std::ofstream ofs(out, std::ios::out | std::ios::app);
        if (ofs)
        {
            std::ostringstream ss;
            ss << "Unhandled std::exception: " << ex.what() << "\n";
            ofs << ss.str();
            ofs.close();
        }
        return -1;
    }
    catch (...)
    {
        const char* dir = "build\\logs";
        CreateDirectoryA("build", NULL);
        CreateDirectoryA(dir, NULL);
        const char* out = "build\\logs\\crash.txt";
        std::ofstream ofs(out, std::ios::out | std::ios::app);
        if (ofs)
        {
            ofs << "Unhandled unknown exception\n";
            ofs.close();
        }
        return -1;
    }
}
#pragma warning(pop)

// Also provide a console main entry so builds that expect a console subsystem link correctly.
int main()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    return RunApp(hInstance);
}
