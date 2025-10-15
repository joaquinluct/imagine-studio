#include "Win32TextRenderer.h"

#include <windows.h>

namespace UI {

void Win32TextRenderer::Initialize(HWND hwnd)
{
    // Create a default font (Segoe UI, 14)
    hFont_ = CreateFontW(-14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Segoe UI");
}

void Win32TextRenderer::Shutdown()
{
    if (hFont_)
    {
        DeleteObject(hFont_);
        hFont_ = nullptr;
    }
}

void Win32TextRenderer::DrawText(HDC hdc, const std::wstring& text, int x, int y)
{
    HFONT old = (HFONT)SelectObject(hdc, hFont_);
    TextOutW(hdc, x, y, text.c_str(), (int)text.size());
    SelectObject(hdc, old);
}

} // namespace UI
