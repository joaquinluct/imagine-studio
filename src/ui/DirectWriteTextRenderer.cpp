#include "DirectWriteTextRenderer.h"

#include <iostream>
#pragma comment(lib, "dwrite.lib")

namespace UI {

DirectWriteTextRenderer::DirectWriteTextRenderer() {}
DirectWriteTextRenderer::~DirectWriteTextRenderer() { Shutdown(); }

bool DirectWriteTextRenderer::IsAvailable() const
{
    return factory_.Get() != nullptr && textFormat_.Get() != nullptr;
}

void DirectWriteTextRenderer::Initialize(HWND hwnd)
{
    HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(factory_.GetAddressOf()));
    if (FAILED(hr))
    {
        std::cerr << "DirectWrite: failed to create factory\n";
        return;
    }

    hr = factory_->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 14.0f, L"en-US", textFormat_.GetAddressOf());
    if (FAILED(hr))
    {
        std::cerr << "DirectWrite: failed to create text format\n";
        return;
    }

    // success -> available
}

void DirectWriteTextRenderer::Shutdown()
{
    textFormat_.Reset();
    factory_.Reset();
}

void DirectWriteTextRenderer::DrawText(HDC hdc, const std::wstring& text, int x, int y)
{
    // For now, fallback to GDI text drawing. Integration with Direct2D is future work.
    HFONT font = CreateFontW(-14,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,VARIABLE_PITCH,L"Segoe UI");
    HFONT old = (HFONT)SelectObject(hdc, font);
    TextOutW(hdc, x, y, text.c_str(), (int)text.size());
    SelectObject(hdc, old);
    DeleteObject(font);
}

} // namespace UI
