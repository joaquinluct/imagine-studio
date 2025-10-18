#include "Transform.h"

namespace Scene {

Transform::Transform()
    : m_position(0.0f, 0.0f, 0.0f)
    , m_rotation(0.0f, 0.0f, 0.0f)
    , m_scale(1.0f, 1.0f, 1.0f)
{
}

Transform::~Transform()
{
}

DirectX::XMMATRIX Transform::GetLocalMatrix() const
{
    using namespace DirectX;

    // Compute TRS matrix (Translate * Rotate * Scale)
    XMMATRIX scaleMatrix = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
    XMMATRIX translationMatrix = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

    // TRS order: Scale -> Rotate -> Translate
    return scaleMatrix * rotationMatrix * translationMatrix;
}

DirectX::XMMATRIX Transform::GetWorldMatrix() const
{
    // For now, local == world (no parent transform)
    // TODO: Implement parent-child hierarchy in H3
    return GetLocalMatrix();
}

} // namespace Scene
