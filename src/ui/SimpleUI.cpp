#include "SimpleUI.h"
#include <iostream>
#include <windows.h>
#include <wingdi.h>
#include <memory>

namespace UI {

void SimpleUI::Initialize(HWND hwnd)
{
    hwnd_ = hwnd;
    std::cout << "SimpleUI: Initialize\n";
}

void SimpleUI::Shutdown()
{
    std::cout << "SimpleUI: Shutdown\n";
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
    FillRect(memDC, &RECT{0,0,panelW,panelH}, brush);
    DeleteObject(brush);

    // Draw text
    SetBkMode(memDC, TRANSPARENT);
    SetTextColor(memDC, RGB(255,255,255));
    TextOut(memDC, 10, 10, L"Hola Mundo", 9);

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

} // namespace UI
