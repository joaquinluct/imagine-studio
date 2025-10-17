#pragma once

#include <windows.h>
#include <unordered_map>

namespace Platform {

class InputManager {
public:
    InputManager();
    ~InputManager();
    
    // Update input state (call once per frame before processing input)
    void Update();
    
    // Check if a key was pressed this frame (rising edge detection)
    bool IsKeyPressed(int vkCode);
    
    // Check if a key is currently down (held)
    bool IsKeyDown(int vkCode);
    
private:
    // Previous frame key states (for edge detection)
    std::unordered_map<int, bool> m_previousKeyStates;
    
    // Current frame key states
    std::unordered_map<int, bool> m_currentKeyStates;
};

} // namespace Platform
