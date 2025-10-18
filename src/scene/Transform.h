#pragma once

#include "Component.h"
#include <DirectXMath.h>

namespace Scene {

/// <summary>
/// Transform Component - Position, Rotation, Scale (AAA standard)
/// Uses DirectXMath for SIMD optimization
/// </summary>
class Transform : public Component {
public:
    Transform();
    ~Transform();

    // Position
    DirectX::XMFLOAT3 GetPosition() const { return m_position; }
    void SetPosition(const DirectX::XMFLOAT3& position) { m_position = position; }
    void SetPosition(float x, float y, float z) { m_position = DirectX::XMFLOAT3(x, y, z); }

    // Rotation (Euler angles in radians)
    DirectX::XMFLOAT3 GetRotation() const { return m_rotation; }
    void SetRotation(const DirectX::XMFLOAT3& rotation) { m_rotation = rotation; }
    void SetRotation(float x, float y, float z) { m_rotation = DirectX::XMFLOAT3(x, y, z); }

    // Scale
    DirectX::XMFLOAT3 GetScale() const { return m_scale; }
    void SetScale(const DirectX::XMFLOAT3& scale) { m_scale = scale; }
    void SetScale(float x, float y, float z) { m_scale = DirectX::XMFLOAT3(x, y, z); }
    void SetUniformScale(float scale) { m_scale = DirectX::XMFLOAT3(scale, scale, scale); }

    // Matrix computation
    DirectX::XMMATRIX GetLocalMatrix() const;
    DirectX::XMMATRIX GetWorldMatrix() const;

private:
    DirectX::XMFLOAT3 m_position;  // Position in world space
    DirectX::XMFLOAT3 m_rotation;  // Rotation in Euler angles (radians)
    DirectX::XMFLOAT3 m_scale;     // Scale factors (1.0 = no scale)
};

} // namespace Scene
