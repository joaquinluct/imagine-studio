#include "InputManager.h"

namespace Platform {

InputManager::InputManager()
{
    // Reserve space for common virtual key codes to avoid rehashing
    m_previousKeyStates.reserve(256);
    m_currentKeyStates.reserve(256);
}

InputManager::~InputManager()
{
    // Cleanup (maps will be destroyed automatically)
}

void InputManager::Update()
{
    // Store previous frame state
    m_previousKeyStates = m_currentKeyStates;
    
    // Poll current state for all virtual key codes we track
    // We only track keys that have been queried before to avoid polling all 256 keys
    for (auto& pair : m_currentKeyStates)
    {
        // GetAsyncKeyState returns high-order bit set if key is down
        short keyState = GetAsyncKeyState(pair.first);
        pair.second = (keyState & 0x8000) != 0;
    }
    
    // v1.5.0 H2.3 - Update mouse state
    POINT currentPos;
    if (GetCursorPos(&currentPos))
    {
        if (!m_mouseInitialized)
        {
            m_lastMousePos = currentPos;
            m_mouseInitialized = true;
        }
        m_lastMousePos = currentPos;
    }
    
    // Reset mouse wheel (will be updated by WM_MOUSEWHEEL message)
    m_mouseWheel = 0;
}

bool InputManager::IsKeyPressed(int vkCode)
{
    // Key pressed = key is down now AND was not down last frame (rising edge)
    bool isDownNow = IsKeyDown(vkCode);
    bool wasDownBefore = m_previousKeyStates.count(vkCode) > 0 && m_previousKeyStates[vkCode];
    
    return isDownNow && !wasDownBefore;
}

bool InputManager::IsKeyDown(int vkCode)
{
    // Check if key is currently down
    // GetAsyncKeyState returns high-order bit set if key is down
    short keyState = GetAsyncKeyState(vkCode);
    bool isDown = (keyState & 0x8000) != 0;
    
    // Update current state (lazy initialization)
    m_currentKeyStates[vkCode] = isDown;
    
    return isDown;
}

// v1.5.0 H2.3 - Mouse input implementation
void InputManager::GetMouseDelta(int& deltaX, int& deltaY)
{
    POINT currentPos;
    if (GetCursorPos(&currentPos))
    {
        deltaX = currentPos.x - m_lastMousePos.x;
        deltaY = currentPos.y - m_lastMousePos.y;
        m_lastMousePos = currentPos;
    }
    else
    {
        deltaX = 0;
        deltaY = 0;
    }
}

int InputManager::GetMouseWheel()
{
    return m_mouseWheel;
}

bool InputManager::IsMouseButtonDown(int button)
{
    // button: 0 = left (VK_LBUTTON), 1 = right (VK_RBUTTON), 2 = middle (VK_MBUTTON)
    int vkCode = VK_LBUTTON;
    if (button == 1) vkCode = VK_RBUTTON;
    else if (button == 2) vkCode = VK_MBUTTON;
    
    short keyState = GetAsyncKeyState(vkCode);
    return (keyState & 0x8000) != 0;
}

void InputManager::OnMouseWheel(int delta)
{
    // Accumulate wheel delta (called from WM_MOUSEWHEEL handler)
    m_mouseWheel += delta;
}

} // namespace Platform
