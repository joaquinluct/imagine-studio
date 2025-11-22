// v2.1.0 H1.5 - Simple PBR Pixel Shader
// Samples 5 PBR textures and outputs albedo (for now)
// Textures: t0=albedo, t1=normal, t2=metallic, t3=roughness, t4=ao

Texture2D AlbedoTexture    : register(t0);
Texture2D NormalTexture    : register(t1);
Texture2D MetallicTexture  : register(t2);
Texture2D RoughnessTexture : register(t3);
Texture2D AOTexture        : register(t4);

SamplerState LinearSampler : register(s0);

struct PSInput
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float2 uv  : TEXCOORD0;
};

float4 PSMain(PSInput input) : SV_TARGET
{
    // Sample albedo texture
    float4 albedo = AlbedoTexture.Sample(LinearSampler, input.uv);
    
    // For now, just return albedo (no lighting)
    // In H1.6 we'll add more complex PBR lighting
    return albedo;
}
