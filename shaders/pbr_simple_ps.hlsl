// v2.1.0 H1.6 - PBR Pixel Shader with Texture Sampling
// Samples 5 PBR textures: Albedo, Normal, Roughness, Metallic, AO

// Textures (bound via root parameter 1 - descriptor table)
Texture2D AlbedoTexture    : register(t0);
Texture2D NormalTexture    : register(t1);
Texture2D RoughnessTexture : register(t2);
Texture2D MetallicTexture  : register(t3);
Texture2D AOTexture        : register(t4);

// Sampler (static sampler in root signature)
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
    
    // For H1.6, we just return the albedo texture
    // Full PBR lighting will be implemented in H2.x
    return albedo;
}
