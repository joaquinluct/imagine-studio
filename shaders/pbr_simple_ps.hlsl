// v2.1.0 H1.5 - Simple PBR Pixel Shader (TEMPORAL - sin texturas)
// Outputs vertex color until H1.6 implements texture binding
// TODO H1.6: Samplear texturas PBR reales (t0-t4)

struct PSInput
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float2 uv  : TEXCOORD0;
};

float4 PSMain(PSInput input) : SV_TARGET
{
    // TEMPORAL H1.5: Return vertex color (texturas en H1.6)
    // TODO H1.6: Sample albedo texture and return it
    // float4 albedo = AlbedoTexture.Sample(LinearSampler, input.uv);
    // return albedo;
    
    return input.col; // Return vertex color for now
}
