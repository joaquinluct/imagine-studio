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
    
    // Mouse input (v1.5.0 - H2.3)
    void GetMouseDelta(int& deltaX, int& deltaY); // Get mouse movement since last frame
    int GetMouseWheel(); // Get mouse wheel delta (positive = scroll up, negative = scroll down)
    bool IsMouseButtonDown(int button); // 0 = left, 1 = right, 2 = middle
    
    // Event handlers (called by Window message proc) - H2.3
    void OnMouseWheel(int delta); // Called from WM_MOUSEWHEEL handler
    
private:
    // Previous frame key states (for edge detection)
    std::unordered_map<int, bool> m_previousKeyStates;
    
    // Current frame key states
    std::unordered_map<int, bool> m_currentKeyStates;
    
    // Mouse state (v1.5.0 - H2.3)
    POINT m_lastMousePos = {};
    int m_mouseWheel = 0;
    bool m_mouseInitialized = false;
};

} // namespace Platform
