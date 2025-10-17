#include "Input.h"
namespace Platform {

void InputManager::Update(HWND hwnd)
{
    // Store previous frame key states for edge detection
    for (int i = 0; i < 256; ++i)
        prevKeyStates_[i] = keyStates_[i];

    // sample keyboard
    for (int i = 0; i < 256; ++i)
        keyStates_[i] = GetAsyncKeyState(i);

    // sample mouse
    mouseButtonStates_[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0;
    mouseButtonStates_[1] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? 1 : 0;
    mouseButtonStates_[2] = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) ? 1 : 0;

    POINT p = {};
    if (GetCursorPos(&p) && ScreenToClient(hwnd, &p))
    {
        mouseX_ = p.x;
        mouseY_ = p.y;
    }
}

bool InputManager::IsKeyDown(int vkey) const
{
    return (keyStates_[vkey] & 0x8000) != 0;
}

bool InputManager::IsKeyPressed(int vkey) const
{
    // Rising edge: key is down this frame but was not down previous frame
    bool isDownNow = (keyStates_[vkey] & 0x8000) != 0;
    bool wasDownBefore = (prevKeyStates_[vkey] & 0x8000) != 0;
    return isDownNow && !wasDownBefore;
}

bool InputManager::IsMouseButtonDown(int button) const
{
    if (button < 0 || button >= 3) return false;
    return mouseButtonStates_[button] != 0;
}

void InputManager::GetMousePosition(int& x, int& y) const
{
    x = mouseX_;
    y = mouseY_;
}

} // namespace Platform
