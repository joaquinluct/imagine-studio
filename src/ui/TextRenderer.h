#pragma once

#include <windows.h>
#include <string>

namespace UI {

class TextRenderer {
public:
    virtual ~TextRenderer() {}
    virtual void Initialize(HWND hwnd) = 0;
    virtual void Shutdown() = 0;
    virtual void DrawText(HDC hdc, const std::wstring& text, int x, int y) = 0;
};

} // namespace UI
