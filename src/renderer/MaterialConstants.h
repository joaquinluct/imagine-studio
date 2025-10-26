#pragma once

#include <DirectXMath.h>

namespace Rendering {

// Material Constant Buffer (register b1)
// Matches MaterialCB in pbr_ps.hlsl
struct MaterialConstants {
    DirectX::XMFLOAT4 albedoColor;    // RGBA (default: 1,1,1,1)
    float metallic;                   // 0.0-1.0
    float roughness;                  // 0.0-1.0
    float padding[2];                 // Padding to 16-byte alignment

    MaterialConstants()
        : albedoColor(1.0f, 1.0f, 1.0f, 1.0f)
        , metallic(0.0f)
        , roughness(0.5f)
        , padding{0.0f, 0.0f}
    {
    }
};

// Light Constant Buffer (register b2)
// Matches LightCB in pbr_ps.hlsl
struct LightConstants {
    DirectX::XMFLOAT3 lightDirection; // Directional light direction (world space)
    float lightIntensity;             // Light intensity (default: 1.0)
    DirectX::XMFLOAT3 lightColor;     // Light color (default: 1,1,1)
    float padding1;
    DirectX::XMFLOAT3 cameraPosition; // Camera position (world space)
    float padding2;

    LightConstants()
        : lightDirection(0.0f, -1.0f, 0.0f)
        , lightIntensity(1.0f)
        , lightColor(1.0f, 1.0f, 1.0f)
        , padding1(0.0f)
        , cameraPosition(0.0f, 0.0f, 0.0f)
        , padding2(0.0f)
    {
    }
};

// Per-Object Constant Buffer (register b0)
// Matches PerObjectCB in pbr_vs.hlsl
struct PerObjectConstants {
    DirectX::XMFLOAT4X4 worldMatrix;
    DirectX::XMFLOAT4X4 viewMatrix;
    DirectX::XMFLOAT4X4 projectionMatrix;
    DirectX::XMFLOAT4X4 worldViewProjectionMatrix;

    PerObjectConstants()
    {
        DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&viewMatrix, DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&projectionMatrix, DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&worldViewProjectionMatrix, DirectX::XMMatrixIdentity());
    }
};

} // namespace Rendering
