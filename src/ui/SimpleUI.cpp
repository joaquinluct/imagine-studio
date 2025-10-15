#include "DirectWriteTextRenderer.h"
#include "SimpleUI.h"
#include "Win32TextRenderer.h"
#include "../tools/Profiler.h"

#include <cwchar>
#include <iostream>
#include <memory>
#include <windows.h>
#include <wingdi.h>
// Ensure Msimg32 is linked when building the Visual Studio project (AlphaBlend)
#pragma comment(lib, "Msimg32.lib")

namespace UI {

void SimpleUI::Initialize(HWND hwnd)
{
    hwnd_ = hwnd;
    // Prefer DirectWrite renderer if available
    auto* dw = new UI::DirectWriteTextRenderer();
    dw->Initialize(hwnd_);
    if (dw->IsAvailable())
    {
        textRenderer_ = dw;
    }
    else
    {
        delete dw;
        UI::TextRenderer* w32 = new UI::Win32TextRenderer();
        w32->Initialize(hwnd_);
        textRenderer_ = w32;
    }
    std::cout << "SimpleUI: Initialize\n";
}

void SimpleUI::Shutdown()
{
    std::cout << "SimpleUI: Shutdown\n";
    if (textRenderer_) { textRenderer_->Shutdown(); delete textRenderer_; textRenderer_ = nullptr; }
    hwnd_ = nullptr;
}

void SimpleUI::Draw()
{
    if (!hwnd_) return;

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd_, &ps);

    // Create a compatible DC and bitmap for alpha blending
    HDC memDC = CreateCompatibleDC(hdc);
    RECT rcClient;
    GetClientRect(hwnd_, &rcClient);

    int panelX = 10;
    int panelY = 10;
    int panelW = 400;
    int panelH = 100;

    HBITMAP hbm = CreateCompatibleBitmap(hdc, panelW, panelH);
    HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, hbm);

    // Fill with semi-transparent gray (we'll use AlphaBlend)
    HBRUSH brush = CreateSolidBrush(RGB(100, 100, 100));
    RECT fillRect = { 0, 0, panelW, panelH };
    FillRect(memDC, &fillRect, brush);
    DeleteObject(brush);

    // Draw text
    SetBkMode(memDC, TRANSPARENT);
    SetTextColor(memDC, RGB(255,255,255));
    if (textRenderer_) textRenderer_->DrawText(memDC, L"Hola Mundo", 10, 10);

    // Blend onto main DC
    BLENDFUNCTION bf = {};
    bf.BlendOp = AC_SRC_OVER;
    bf.SourceConstantAlpha = 160; // ~62% opacity

    BOOL res = AlphaBlend(hdc, panelX, panelY, panelW, panelH, memDC, 0, 0, panelW, panelH, bf);

    // Cleanup
    SelectObject(memDC, oldBmp);
    DeleteObject(hbm);
    DeleteDC(memDC);

    EndPaint(hwnd_, &ps);
}

void SimpleUI::DrawOverlay()
{
    if (!hwnd_) return;
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd_, &ps);
    // draw FPS at top-right
    double fps = Tools::Profiler::Instance().GetFPS();
    wchar_t buf[128];
    swprintf_s(buf, L"FPS: %.1f", fps);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0,255,0));
    if (textRenderer_) textRenderer_->DrawText(hdc, buf, 600, 10);
    EndPaint(hwnd_, &ps);
}

} // namespace UI
