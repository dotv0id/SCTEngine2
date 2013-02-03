// filename: color.fx

// Globals
matrix worldMatrix;
matrix viewMatrix;
matrix projectionMatrix;

// Vertex Shader input struct
struct VS_IN
{
	float3 posL    : POSITION;
	float3 normalL : NORMAL;
	float2 texC    : TEXCOORD;
};

// Vertex Shader output struct (input struct of the pixel shader)
struct VS_OUT
{
	float4 posH    : SV_POSITION;
    float3 posW    : POSITION;
    float3 normalW : NORMAL;
    float2 texC    : TEXCOORD;
};

// Vertex Shader
// ---------------------------------
VS_OUT VS(VS_IN vIn)
{
/*
    PixelInputType output;
    
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the input color for the pixel shader to use.
    output.color = input.color;
    
    return output;
*/
}

// The pixel Shader
float4 PS(VS_OUT pIn) : SV_Target  // , uniform bool bTexture, uniform bool bSpecular
{
}

// Pixel Shader
// ---------------------------------
float4 ColorPixelShader(PixelInputType input) : SV_Target
{
    return input.color;
}


// Techniques
// ---------------------------------
technique10 ColorTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, ColorVertexShader()));
        SetPixelShader(CompileShader(ps_4_0, ColorPixelShader()));
        SetGeometryShader(NULL);
    }
}