#pragma once

namespace Renderer {

// v1.5.0 H2.1 - Camera class for 3D rendering (View + Projection matrices)
// Supports perspective projection and look-at view transformation
class Camera {
public:
    Camera();
    ~Camera() = default;

    // Camera positioning
    void SetPosition(float x, float y, float z);
    void SetTarget(float x, float y, float z);
    void SetUpVector(float x, float y, float z);
    
    // Projection configuration
    void SetPerspective(float fovDegrees, float aspectRatio, float nearPlane, float farPlane);
    
    // Matrix getters (row-major for DX12 shader compatibility)
    const float* GetViewMatrix() const { return m_viewMatrix; }
    const float* GetProjectionMatrix() const { return m_projectionMatrix; }
    
    // Position getters
    void GetPosition(float& x, float& y, float& z) const;
    void GetTarget(float& x, float& y, float& z) const;

private:
    // Camera vectors
    float m_position[3] = { 0.0f, 0.0f, -5.0f };  // Camera position (default: back 5 units)
    float m_target[3] = { 0.0f, 0.0f, 0.0f };     // Look-at target (default: origin)
    float m_up[3] = { 0.0f, 1.0f, 0.0f };         // Up vector (default: +Y)
    
    // Projection parameters
    float m_fovDegrees = 45.0f;
    float m_aspectRatio = 16.0f / 9.0f;
    float m_nearPlane = 0.1f;
    float m_farPlane = 1000.0f;
    
    // Matrices (row-major for DX12 shader compatibility)
    float m_viewMatrix[16] = {};
    float m_projectionMatrix[16] = {};
    
    // Internal matrix computation
    void UpdateViewMatrix();
    void UpdateProjectionMatrix();
};

} // namespace Renderer
