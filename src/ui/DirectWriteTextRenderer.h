#pragma once

#include "TextRenderer.h"

#include <dwrite.h>
#include <windows.h>
#include <wrl.h>
namespace UI {

class DirectWriteTextRenderer : public TextRenderer {
public:
    DirectWriteTextRenderer();
    ~DirectWriteTextRenderer() override;

    void Initialize(HWND hwnd) override;
    void Shutdown() override;
    void DrawText(HDC hdc, const std::wstring& text, int x, int y) override;

    // Whether DirectWrite initialized successfully
    bool IsAvailable() const;

private:
    Microsoft::WRL::ComPtr<IDWriteFactory> factory_;
    Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat_;
};

} // namespace UI
