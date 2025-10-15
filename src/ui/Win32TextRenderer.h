#pragma once

#include "TextRenderer.h"

#include <windows.h>
namespace UI {

class Win32TextRenderer : public TextRenderer {
public:
    Win32TextRenderer() = default;
    ~Win32TextRenderer() override = default;

    void Initialize(HWND hwnd) override;
    void Shutdown() override;
    void DrawText(HDC hdc, const std::wstring& text, int x, int y) override;

private:
    HFONT hFont_ = nullptr;
};

} // namespace UI
