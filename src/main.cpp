// Application entry using Platform::Window
#include "assets/AssetManager.h"
#include "core/Log.h"
#include "editor/EditorUI.h"
#include "editor/Viewport.h"    // v1.5.0 H3.1 - Viewport panel
#include "jobs/TaskGraph.h"
#include "jobs/ThreadPool.h"
#include "platform/InputManager.h"
#include "platform/Window.h"
#include "renderer/Camera.h"     // v1.5.0 H2.3 - Camera controls
#include "renderer/DX12Renderer.h"
#include "scene/Scene.h"
#include "tools/Profiler.h"
#include "ui/SimpleUI.h"

#include <chrono>
#include <fstream>
#include <sstream>
#include <windows.h>

// ImGui headers (Dear ImGui - Editor UI Framework)
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

// DirectX 12 headers (for ImGui backend initialization)
#if defined(_WIN32) && defined(_MSC_VER)
#include <d3d12.h>
#endif

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

    // Log HWND for diagnosis
    {
        std::ostringstream ss; ss << "Main window HWND=" << reinterpret_cast<void*>(window.GetHWND()); CORE_LOG_INFO(ss.str());
    }

    CORE_LOG_INFO(std::string("Render window should be visible (HWND=)") + std::to_string(reinterpret_cast<uintptr_t>(window.GetHWND())));

    // Setup Dear ImGui context (v1.3.0 - Editor UI Framework)
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    // Enable docking (H4 - Editor Panels & Docking)
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    // Setup Dear ImGui style (dark theme)
    ImGui::StyleColorsDark();
    
    // ✅ AAA STANDARD: Fondo semitransparente para ver el render 3D debajo
    // Esto permite que la UI del editor no tape completamente la escena renderizada
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.70f);  // Fondo ventanas (70% opacidad)
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);   // Fondo child windows (transparente)
    style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);  // Fondo dockspace vacío (50% opacidad)
    
    CORE_LOG_INFO("ImGui style configured: semi-transparent background (Unity/Unreal style)");
    
    // ✅ CRÍTICO: Añadir font por defecto ANTES de Build()
    io.Fonts->AddFontDefault();
    
    // ✅ CRÍTICO: Build font atlas ANTES de inicializar backends
    // Esto es necesario cuando el backend no soporta texturas dinámicas automáticamente
    io.Fonts->Build();
    
    // ✅ CRÍTICO: Configurar flag RendererHasTextures DESPUÉS de Build
    // (El flag indica que el backend puede manejar texturas, pero el atlas ya debe estar construido)
    io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures;
    
    // Initialize Win32 backend (REQUERIDO - debe estar ANTES de DX12 backend)
    ImGui_ImplWin32_Init(hwnd);
    
    CORE_LOG_INFO("ImGui context created and initialized (v1.3.0 - docking enabled)");

    // Simple message loop
    Platform::InputManager input;
    Renderer::DX12Renderer renderer;
    UI::SimpleUI ui;

    renderer.Initialize(hwnd);
    
    // Create Scene (H4 - Editor Integration with Scene Graph)
    Scene::Scene scene("Main Scene");
    CORE_LOG_INFO("Scene created: Main Scene");
    
    // Create default entities for demo
    Scene::Entity* camera = scene.CreateEntity("Main Camera");
    Scene::Entity* light = scene.CreateEntity("Directional Light");
    Scene::Entity* geometry = scene.CreateEntity("Quad");
    
    CORE_LOG_INFO("Default entities created: Main Camera, Directional Light, Quad");

    // Setup Platform/Renderer backends (H2.2 - ImGui DX12 Backend)
#if defined(_WIN32) && defined(_MSC_VER)
    // Get ImGui SRV descriptor heap and device from renderer
    ID3D12DescriptorHeap* imguiSrvHeap = renderer.GetImGuiSrvHeap();
    ID3D12Device* device = renderer.GetDevice();
    
    if (imguiSrvHeap && device)
    {
        // Get CPU and GPU descriptor handles for ImGui font atlas
        D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = imguiSrvHeap->GetCPUDescriptorHandleForHeapStart();
        D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = imguiSrvHeap->GetGPUDescriptorHandleForHeapStart();
        
        // Initialize ImGui DX12 backend
        ImGui_ImplDX12_Init(
            device,                           // ID3D12Device*
            2,                                // num frames in flight (double buffering)
            DXGI_FORMAT_R8G8B8A8_UNORM,      // RTV format (matches swap chain)
            imguiSrvHeap,                     // ID3D12DescriptorHeap* (SRV heap)
            cpuHandle,                        // CPU handle for font atlas
            gpuHandle                         // GPU handle for font atlas
        );
        
        CORE_LOG_INFO("ImGui DX12 backend initialized");
    }
    else
    {
        CORE_LOG_WARN("ImGui DX12 backend not initialized: device or SRV heap not available");
    }
#else
    CORE_LOG_WARN("ImGui DX12 backend not initialized: platform not supported");
#endif
    
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
        input.Update();

        // Example: if Escape pressed, exit
        if (input.IsKeyDown(VK_ESCAPE))
            break;
        
        // H2.3: Toggle UI visibility with F1 key
        if (input.IsKeyPressed(VK_F1))
        {
            renderer.ToggleUI();
            CORE_LOG_INFO(std::string("UI visibility toggled: ") + (renderer.IsUIVisible() ? "visible" : "hidden"));
        }
        
        // v1.5.0 H2.3: Camera controls (orbit/pan/zoom)
        // v1.5.0 H4.1: Restringir controles solo cuando Viewport tiene hover
        Renderer::Camera* camera = renderer.GetCamera();
        Editor::Viewport* viewport = Editor::EditorUI::GetViewport();
        
        if (camera && viewport)
        {
            // v1.5.0 H4.3: Actualizar aspect ratio de cámara según tamaño de Viewport
            float viewportWidth, viewportHeight;
            viewport->GetSize(viewportWidth, viewportHeight);
            
            if (viewportWidth > 0.0f && viewportHeight > 0.0f)
            {
                float aspectRatio = viewportWidth / viewportHeight;
                camera->SetPerspective(45.0f, aspectRatio, 0.1f, 1000.0f);
            }
            
            // Controles solo si Viewport tiene hover
            if (viewport->IsHovered())
            {
                // Orbit: Right mouse button
                if (input.IsMouseButtonDown(1)) // Right button
                {
                    int deltaX, deltaY;
                    input.GetMouseDelta(deltaX, deltaY);
                    
                    // Convert pixel delta to radians (sensitivity)
                    float yaw = deltaX * 0.005f;   // Horizontal rotation
                    float pitch = -deltaY * 0.005f; // Vertical rotation (inverted Y)
                    
                    camera->Orbit(yaw, pitch);
                }
                
                // Pan: Middle mouse button
                if (input.IsMouseButtonDown(2)) // Middle button
                {
                    int deltaX, deltaY;
                    input.GetMouseDelta(deltaX, deltaY);
                    
                    camera->Pan(static_cast<float>(deltaX), static_cast<float>(-deltaY)); // Inverted Y
                }
                
                // Zoom: +/- keys (alternative to mouse wheel)
                if (input.IsKeyDown(VK_ADD) || input.IsKeyDown(VK_OEM_PLUS)) // + or =
                {
                    camera->Zoom(1.0f); // Zoom in
                }
                if (input.IsKeyDown(VK_SUBTRACT) || input.IsKeyDown(VK_OEM_MINUS)) // - or _
                {
                    camera->Zoom(-1.0f); // Zoom out
                }
            }
        }

        // Poll for loaded assets and notify renderer
        std::string loadedPath;
        while (assets.PopLoaded(loadedPath))
        {
            renderer.OnAssetLoaded(loadedPath);
        }

        // ✅ CRITICAL FIX: Always call ImGui::NewFrame/Render (even when UI hidden)
        // This ensures ImGui::GetDrawData() returns empty data when UI is hidden
        // Start ImGui frame
        ImGui_ImplWin32_NewFrame();
        ImGui_ImplDX12_NewFrame();
        ImGui::NewFrame();
        
        // ✅ Only render UI panels if visible (AAA standard - Unity/Unreal style)
        if (renderer.IsUIVisible())
        {
            // ✅ AAA STANDARD: Dockspace principal ANTES de panels (Unity/Unreal style)
            // Permite docking flexible de todos los panels del editor
            ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
            
            // v1.5.0 H3.1 - Update viewport with render target SRV
            Editor::Viewport* viewport = Editor::EditorUI::GetViewport();
            if (viewport)
            {
#if defined(_WIN32) && defined(_MSC_VER)
                // v1.6.0 DEV-002.6: Get scene render target SRV (AAA architecture)
                D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = renderer.GetSceneRenderTargetSRV();
                viewport->SetRenderTargetSRV(reinterpret_cast<void*>(srvHandle.ptr));
#endif
            }
            
            // ✅ H4: Render all editor panels with Scene integration
            Editor::EditorUI::RenderAllPanels(&scene);
            
            // ✅ AAA STANDARD: Demo window solo en debug builds
            #ifdef _DEBUG
                // ImGui::ShowDemoWindow();  // Deshabilitado, ahora usamos EditorUI
            #endif
        }
        
        // ✅ CRITICAL: Always call Render() to finalize draw data (even if empty)
        ImGui::Render();

        // Renderer renderiza frame (incluye UIPass con ImGui draw data si m_uiVisible)
        renderer.RenderFrame();
        ui.Draw();
        ui.DrawOverlay();
        
        auto frameEnd = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(frameEnd - frameStart).count();
        Tools::Profiler::Instance().RecordFrame(ms);
    }

    assets.Shutdown();
    assets.DumpMetrics();
    ui.Shutdown();
    renderer.Shutdown();
    
    // Cleanup Dear ImGui (H2.2 - shutdown backends first, then context)
#if defined(_WIN32) && defined(_MSC_VER)
    ImGui_ImplDX12_Shutdown();
    CORE_LOG_INFO("ImGui DX12 backend shutdown");
#endif
    ImGui_ImplWin32_Shutdown();
    CORE_LOG_INFO("ImGui Win32 backend shutdown");
    ImGui::DestroyContext();
    CORE_LOG_INFO("ImGui context destroyed");

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

