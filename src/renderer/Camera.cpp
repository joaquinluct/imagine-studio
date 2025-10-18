#include "Camera.h"
#include "core/Log.h"

#include <cmath>

// Use DirectXMath for matrix operations (standard Microsoft library)
#if defined(_WIN32) && defined(_MSC_VER)
#include <DirectXMath.h>
using namespace DirectX;
#endif

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
#if defined(_WIN32) && defined(_MSC_VER)
    // Use DirectXMath standard look-at matrix (left-handed, row-major)
    XMVECTOR eye = XMVectorSet(m_position[0], m_position[1], m_position[2], 1.0f);
    XMVECTOR target = XMVectorSet(m_target[0], m_target[1], m_target[2], 1.0f);
    XMVECTOR up = XMVectorSet(m_up[0], m_up[1], m_up[2], 0.0f);
    
    XMMATRIX view = XMMatrixLookAtLH(eye, target, up);
    
    // Store the matrix (DirectXMath uses row-major by default)
    XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(m_viewMatrix), view);
#else
    // Fallback: identity matrix
    for (int i = 0; i < 16; ++i)
        m_viewMatrix[i] = 0.0f;
    m_viewMatrix[0] = m_viewMatrix[5] = m_viewMatrix[10] = m_viewMatrix[15] = 1.0f;
#endif
}

void Camera::UpdateProjectionMatrix()
{
#if defined(_WIN32) && defined(_MSC_VER)
    // Use DirectXMath standard perspective matrix (left-handed, row-major)
    const float fovRadians = m_fovDegrees * 3.14159265359f / 180.0f;
    
    XMMATRIX proj = XMMatrixPerspectiveFovLH(
        fovRadians,
        m_aspectRatio,
        m_nearPlane,
        m_farPlane
    );
    
    // Store the matrix (DirectXMath uses row-major by default)
    XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(m_projectionMatrix), proj);
#else
    // Fallback: identity matrix
    for (int i = 0; i < 16; ++i)
        m_projectionMatrix[i] = 0.0f;
    m_projectionMatrix[0] = m_projectionMatrix[5] = m_projectionMatrix[10] = m_projectionMatrix[15] = 1.0f;
#endif
}

} // namespace Renderer
