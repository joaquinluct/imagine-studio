#pragma once

#include <windows.h>

namespace UI {

class IUI {
public:
    virtual ~IUI() {}
    // Initialize with the application window handle
    virtual void Initialize(HWND hwnd) = 0;
    virtual void Shutdown() = 0;
    virtual void Draw() = 0;
};

} // namespace UI
