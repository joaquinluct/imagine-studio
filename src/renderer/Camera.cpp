#include "Camera.h"
#include "core/Log.h"

#include <cmath>

namespace Renderer {

// v1.5.0 H2.1 - Camera implementation with View and Projection matrices

Camera::Camera()
{
    // Initialize with default values
    UpdateViewMatrix();
    UpdateProjectionMatrix();
    
    CORE_LOG_INFO("Camera created: position (0, 0, -5), target (0, 0, 0), FOV 45°");
}

void Camera::SetPosition(float x, float y, float z)
{
    m_position[0] = x;
    m_position[1] = y;
    m_position[2] = z;
    UpdateViewMatrix();
}

void Camera::SetTarget(float x, float y, float z)
{
    m_target[0] = x;
    m_target[1] = y;
    m_target[2] = z;
    UpdateViewMatrix();
}

void Camera::SetUpVector(float x, float y, float z)
{
    m_up[0] = x;
    m_up[1] = y;
    m_up[2] = z;
    UpdateViewMatrix();
}

void Camera::SetPerspective(float fovDegrees, float aspectRatio, float nearPlane, float farPlane)
{
    m_fovDegrees = fovDegrees;
    m_aspectRatio = aspectRatio;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
    UpdateProjectionMatrix();
}

void Camera::GetPosition(float& x, float& y, float& z) const
{
    x = m_position[0];
    y = m_position[1];
    z = m_position[2];
}

void Camera::GetTarget(float& x, float& y, float& z) const
{
    x = m_target[0];
    y = m_target[1];
    z = m_target[2];
}

// Helper functions for vector math (inline)
namespace {
    // Normalize vector (in-place)
    inline void Normalize(float v[3])
    {
        float len = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
        if (len > 0.0f)
        {
            v[0] /= len;
            v[1] /= len;
            v[2] /= len;
        }
    }
    
    // Cross product: result = a x b
    inline void Cross(const float a[3], const float b[3], float result[3])
    {
        result[0] = a[1] * b[2] - a[2] * b[1];
        result[1] = a[2] * b[0] - a[0] * b[2];
        result[2] = a[0] * b[1] - a[1] * b[0];
    }
    
    // Dot product
    inline float Dot(const float a[3], const float b[3])
    {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    }
}

void Camera::UpdateViewMatrix()
{
    // Compute forward vector (from position to target)
    float forward[3] = {
        m_target[0] - m_position[0],
        m_target[1] - m_position[1],
        m_target[2] - m_position[2]
    };
    Normalize(forward);
    
    // Compute right vector (cross product of forward and up)
    float right[3];
    Cross(forward, m_up, right);
    Normalize(right);
    
    // Recompute up vector (cross product of right and forward)
    float up[3];
    Cross(right, forward, up);
    Normalize(up);
    
    // Build view matrix (column-major)
    // Column 0 (right)
    m_viewMatrix[0] = right[0];
    m_viewMatrix[1] = up[0];
    m_viewMatrix[2] = -forward[0];
    m_viewMatrix[3] = 0.0f;
    
    // Column 1 (up)
    m_viewMatrix[4] = right[1];
    m_viewMatrix[5] = up[1];
    m_viewMatrix[6] = -forward[1];
    m_viewMatrix[7] = 0.0f;
    
    // Column 2 (forward)
    m_viewMatrix[8] = right[2];
    m_viewMatrix[9] = up[2];
    m_viewMatrix[10] = -forward[2];
    m_viewMatrix[11] = 0.0f;
    
    // Column 3 (translation)
    m_viewMatrix[12] = -Dot(right, m_position);
    m_viewMatrix[13] = -Dot(up, m_position);
    m_viewMatrix[14] = Dot(forward, m_position);
    m_viewMatrix[15] = 1.0f;
}

void Camera::UpdateProjectionMatrix()
{
    // Perspective projection matrix (column-major for DX12)
    // Based on DirectX left-handed coordinate system (Z forward, Y up)
    
    const float fovRadians = m_fovDegrees * 3.14159265359f / 180.0f;
    const float tanHalfFov = std::tan(fovRadians / 2.0f);
    
    const float f = 1.0f / tanHalfFov;
    const float nf = 1.0f / (m_nearPlane - m_farPlane);
    
    // Initialize to zero
    for (int i = 0; i < 16; ++i)
        m_projectionMatrix[i] = 0.0f;
    
    // Column-major perspective matrix (DX12 left-handed)
    m_projectionMatrix[0] = f / m_aspectRatio;  // [0,0]
    m_projectionMatrix[5] = f;                   // [1,1]
    m_projectionMatrix[10] = (m_farPlane + m_nearPlane) * nf;  // [2,2]
    m_projectionMatrix[11] = -1.0f;              // [2,3]
    m_projectionMatrix[14] = (2.0f * m_farPlane * m_nearPlane) * nf;  // [3,2]
}

} // namespace Renderer
