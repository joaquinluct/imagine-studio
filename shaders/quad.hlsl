// quad.hlsl - Basic vertex and pixel shaders for colored quad rendering
// Uses root constants for MVP matrix transformation

// Root constants (16 floats = 64 bytes) bound to register b0
cbuffer Constants : register(b0)
{
    float4x4 mvp; // Model-View-Projection matrix
};

// Input structure for vertex shader
struct VSInput
{
    float3 pos : POSITION; // Vertex position (x, y, z)
    float4 col : COLOR;    // Vertex color (r, g, b, a)
};

// Output structure from vertex shader / input to pixel shader
struct PSInput
{
    float4 pos : SV_POSITION; // Transformed position (clip space)
    float4 col : COLOR;       // Interpolated color
};

// Vertex Shader: Transform vertex position and pass color through
PSInput VSMain(VSInput input)
{
    PSInput output;
    
    // Transform vertex position to clip space using MVP matrix
    output.pos = mul(float4(input.pos, 1.0), mvp);
    
    // Pass vertex color through for interpolation
    output.col = input.col;
    
    return output;
}

// Pixel Shader: Output interpolated color
float4 PSMain(PSInput input) : SV_TARGET
{
    return input.col; // Return interpolated color
}
