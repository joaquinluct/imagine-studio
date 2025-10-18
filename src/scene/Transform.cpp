#include "Transform.h"
#include "Entity.h"

namespace Scene {

Transform::Transform()
    : m_position(0.0f, 0.0f, 0.0f)
    , m_rotation(0.0f, 0.0f, 0.0f)
    , m_scale(1.0f, 1.0f, 1.0f)
    , m_parent(nullptr)
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
    using namespace DirectX;

    XMMATRIX localMatrix = GetLocalMatrix();

    // If no parent, local == world
    if (m_parent == nullptr) {
        return localMatrix;
    }

    // Get parent's transform component
    Transform* parentTransform = m_parent->GetComponent<Transform>();
    if (parentTransform == nullptr) {
        // Parent has no transform, use local only
        return localMatrix;
    }

    // Compute world matrix: parent's world matrix * local matrix
    XMMATRIX parentWorldMatrix = parentTransform->GetWorldMatrix();
    return localMatrix * parentWorldMatrix;
}

} // namespace Scene
