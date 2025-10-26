// PBR Pixel Shader
// Physically Based Rendering with Cook-Torrance BRDF
// Features: Normal Mapping, Metallic-Roughness workflow, Directional Light

// Constant Buffers
cbuffer MaterialCB : register(b1)
{
    float4 AlbedoColor;      // RGBA (default: 1,1,1,1)
    float  Metallic;         // 0.0-1.0
    float  Roughness;        // 0.0-1.0
    float2 Padding;
};

cbuffer LightCB : register(b2)
{
    float3 LightDirection;   // Directional light direction (world space)
    float  LightIntensity;   // Light intensity (default: 1.0)
    float3 LightColor;       // Light color (default: 1,1,1)
    float  Padding2;
    float3 CameraPosition;   // Camera position (world space)
    float  Padding3;
};

// Textures
Texture2D AlbedoTexture    : register(t0);
Texture2D NormalTexture    : register(t1);
Texture2D RoughnessTexture : register(t2);
Texture2D MetallicTexture  : register(t3);
Texture2D AOTexture        : register(t4);

// Sampler
SamplerState LinearSampler : register(s0);

// Pixel Input (Vertex Shader Output)
struct PixelInput
{
    float4 PositionCS : SV_POSITION;
    float3 PositionWS : POSITION;
    float3 NormalWS   : NORMAL;
    float3 TangentWS  : TANGENT;
    float3 BitangentWS: BITANGENT;
    float2 UV         : TEXCOORD0;
};

// Constants
static const float PI = 3.14159265359;
static const float EPSILON = 0.001;

// ===== PBR Functions =====

// Normal Distribution Function (GGX/Trowbridge-Reitz)
float DistributionGGX(float NdotH, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH2 = NdotH * NdotH;
    
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    
    return a2 / max(denom, EPSILON);
}

// Geometry Function (Schlick-GGX)
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    
    float denom = NdotV * (1.0 - k) + k;
    
    return NdotV / max(denom, EPSILON);
}

// Geometry Function (Smith's method)
float GeometrySmith(float NdotV, float NdotL, float roughness)
{
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}

// Fresnel Function (Schlick approximation)
float3 FresnelSchlick(float VdotH, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);
}

// ===== Pixel Shader Main =====

float4 main(PixelInput input) : SV_TARGET
{
    // Sample textures
    float3 albedo = AlbedoTexture.Sample(LinearSampler, input.UV).rgb * AlbedoColor.rgb;
    float3 normalMap = NormalTexture.Sample(LinearSampler, input.UV).rgb;
    float roughness = RoughnessTexture.Sample(LinearSampler, input.UV).r * Roughness;
    float metallic = MetallicTexture.Sample(LinearSampler, input.UV).r * Metallic;
    float ao = AOTexture.Sample(LinearSampler, input.UV).r;
    
    // Normal mapping: Transform normal from tangent space to world space
    // Construct TBN matrix
    float3 N = normalize(input.NormalWS);
    float3 T = normalize(input.TangentWS);
    float3 B = normalize(input.BitangentWS);
    float3x3 TBN = float3x3(T, B, N);
    
    // Convert normal map from [0,1] to [-1,1]
    float3 tangentNormal = normalMap * 2.0 - 1.0;
    
    // Transform to world space
    N = normalize(mul(tangentNormal, TBN));
    
    // Calculate lighting vectors
    float3 V = normalize(CameraPosition - input.PositionWS); // View direction
    float3 L = normalize(-LightDirection);                   // Light direction
    float3 H = normalize(V + L);                             // Halfway vector
    
    // Calculate dot products
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float NdotH = max(dot(N, H), 0.0);
    float VdotH = max(dot(V, H), 0.0);
    
    // Calculate F0 (surface reflection at zero incidence)
    // For dielectrics, F0 = 0.04; for metals, F0 = albedo
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);
    
    // Cook-Torrance BRDF
    float D = DistributionGGX(NdotH, roughness);
    float G = GeometrySmith(NdotV, NdotL, roughness);
    float3 F = FresnelSchlick(VdotH, F0);
    
    // Specular term
    float3 numerator = D * G * F;
    float denominator = 4.0 * NdotV * NdotL + EPSILON;
    float3 specular = numerator / denominator;
    
    // Diffuse term (Lambert)
    float3 kD = (1.0 - F) * (1.0 - metallic); // Metals have no diffuse
    float3 diffuse = kD * albedo / PI;
    
    // Final radiance
    float3 radiance = (diffuse + specular) * LightColor * LightIntensity * NdotL;
    
    // Ambient term (simple ambient occlusion)
    float3 ambient = float3(0.03, 0.03, 0.03) * albedo * ao;
    
    // Final color
    float3 color = ambient + radiance;
    
    // Tone mapping (Reinhard)
    color = color / (color + float3(1.0, 1.0, 1.0));
    
    // Gamma correction
    color = pow(color, float3(1.0/2.2, 1.0/2.2, 1.0/2.2));
    
    return float4(color, AlbedoColor.a);
}
