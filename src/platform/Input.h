#pragma once

#include <cstdint>
#include <windows.h>
namespace Platform {

class InputManager {
public:
    InputManager() = default;
    ~InputManager() = default;

    // Call once per frame to sample input state. Requires the window handle to transform mouse coords.
    void Update(HWND hwnd);

    bool IsKeyDown(int vkey) const;
    bool IsKeyPressed(int vkey) const; // Detects rising edge (key just pressed this frame)
    bool IsMouseButtonDown(int button) const; // VK_LBUTTON, VK_RBUTTON
    void GetMousePosition(int& x, int& y) const;

private:
    short keyStates_[256] = {};
    short prevKeyStates_[256] = {}; // Previous frame key states for edge detection
    short mouseButtonStates_[3] = {}; // left, right, middle
    int mouseX_ = 0;
    int mouseY_ = 0;
};

} // namespace Platform
