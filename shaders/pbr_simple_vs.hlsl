// v2.1.0 H1.5 - Simple PBR Vertex Shader
// Input: Position, Color, UV
// Output: Position, Color, UV (pass-through)

cbuffer Constants : register(b0)
{
    float4x4 mvp;
};

struct VSInput
{
    float3 pos : POSITION;
    float4 col : COLOR;
    float2 uv  : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float2 uv  : TEXCOORD0;
};

PSInput VSMain(VSInput input)
{
    PSInput output;
    output.pos = mul(float4(input.pos, 1.0), mvp);
    output.col = input.col;
    output.uv = input.uv;
    return output;
}
