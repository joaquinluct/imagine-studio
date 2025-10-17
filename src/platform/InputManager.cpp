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

} // namespace Platform
