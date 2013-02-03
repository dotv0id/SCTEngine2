//! ------------------------------------------------------------
//	@File:			DefaultWireframeShader.fx
//   
//	Author: Anastasios 'v0id' Giannakopoulos
//	E-Mail: dotvoidd@gmail.com
//   
//	Description:	Default Shader used to render wireframe
// ------------------------------------------------------------


// Global variables
matrix	gWorld;
matrix	gWVP;
float4	gWireframeColor;


// Rasterizer state
RasterizerState rsWireframe
{ 
	FillMode = WireFrame; 
	CullMode = None;
};

// Blend State -- try
BlendState bsAlphaOverwrite
{
	BlendEnable[0]	= FALSE;
};

// Vertex Input type
struct VertexInput
{
	float3 pos : POSITION;
	// No need of the rest
};

// Pixel Input type
struct PixelInput
{
	float4 posH : SV_POSITION;
	float3 posW	: POSITION;
	// No need of the rest
};


// Vertex Shader
PixelInput WireframeVertexShader(VertexInput input)
{
	PixelInput output;
	
	// Transform to homogeneous clip space.
	output.posH = mul(float4(input.pos, 1.0f), gWVP);

	// Change the position vector to be 4 units for proper matrix calculations.
	//input.position.w = 1.0f;
	
	// Calculate the position of the vertex against the world, view, and projection matrices.
	//output.position = mul(input.position, gWVP);
	
	return output;
}

// Pixel Shader
float4 WireframePixelShader(PixelInput input) : SV_Target
{
	return gWireframeColor;
}


// Technique
technique10 WireframeTechnique
{
    pass pass0
    {
		SetRasterizerState(rsWireframe);
		SetBlendState(bsAlphaOverwrite, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_4_0, WireframeVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, WireframePixelShader()));		
    }
}