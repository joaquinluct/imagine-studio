// PBR Vertex Shader
// Physically Based Rendering with Normal Mapping support
// Input: Position, Normal, Tangent, UV
// Output: World Position, TBN Matrix, UV

// Constant Buffers
cbuffer PerObjectCB : register(b0)
{
    float4x4 WorldMatrix;
    float4x4 ViewMatrix;
    float4x4 ProjectionMatrix;
    float4x4 WorldViewProjectionMatrix;
};

// Vertex Input
struct VertexInput
{
    float3 Position : POSITION;
    float3 Normal   : NORMAL;
    float3 Tangent  : TANGENT;
    float2 UV       : TEXCOORD0;
};

// Vertex Output (Pixel Shader Input)
struct VertexOutput
{
    float4 PositionCS : SV_POSITION;  // Clip space position
    float3 PositionWS : POSITION;     // World space position
    float3 NormalWS   : NORMAL;       // World space normal
    float3 TangentWS  : TANGENT;      // World space tangent
    float3 BitangentWS: BITANGENT;    // World space bitangent
    float2 UV         : TEXCOORD0;    // Texture coordinates
};

// Vertex Shader Main
VertexOutput main(VertexInput input)
{
    VertexOutput output;
    
    // Transform position to world space
    float4 worldPos = mul(float4(input.Position, 1.0), WorldMatrix);
    output.PositionWS = worldPos.xyz;
    
    // Transform position to clip space using pre-calculated WVP matrix
    output.PositionCS = mul(float4(input.Position, 1.0), WorldViewProjectionMatrix);
    
    // Transform normal to world space
    output.NormalWS = normalize(mul(input.Normal, (float3x3)WorldMatrix));
    
    // Transform tangent to world space
    output.TangentWS = normalize(mul(input.Tangent, (float3x3)WorldMatrix));
    
    // Calculate bitangent (cross product of normal and tangent)
    output.BitangentWS = normalize(cross(output.NormalWS, output.TangentWS));
    
    // Pass through texture coordinates
    output.UV = input.UV;
    
    return output;
}
