//! ------------------------------------------------------------
//	@File:			DefaultLightShader.fx
//   
//	Author: Anastasios 'v0id' Giannakopoulos
//	E-Mail: dotvoidd@gmail.com
//   
//	Description:	Default Shader used to render wireframe
// ------------------------------------------------------------


// Global variables
matrix		gWVP;
Texture2D	gShaderTexture;
float4		gPixelColor;


// Sampler States
SamplerState SampleType
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

// Rasterizer State
RasterizerState rsSolid 
{
	CullMode = None;
};

// Blending States
BlendState bsAlphaBlendingState
{
	BlendEnable[0] = TRUE;
	DestBlend = INV_SRC_ALPHA;
};


// Vertex Input type
struct VertexInputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
};

// Pixel Input type
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


// Vertex Shader
PixelInputType FontVertexShader(VertexInputType input)
{
    PixelInputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices
    output.position = mul(float4(input.position, 1.0f), gWVP);
    
	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
	return output;
}


// Pixel Shader
float4 FontPixelShader(PixelInputType input) : SV_Target
{
	float4 color;

	// Sample the texture pixel at this location.
	color = gShaderTexture.Sample(SampleType, input.tex);
	
	// If the color is black on the texture then treat this pixel as transparent.
	if(color.r == 0.0f)
	{
		color.a = 0.0f;
	}
	
	// If the color is other than black on the texture then this is a pixel in the font so draw it using the font pixel color.
	else
	{
		color.rgb = gPixelColor.rgb;
		color.a = 1.0f;
	}

    return color;
}


// Technique
technique10 FontTechnique
{
    pass pass0
    {
		SetRasterizerState(rsSolid);
		SetBlendState(bsAlphaBlendingState, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_4_0, FontVertexShader()));
		SetPixelShader(CompileShader(ps_4_0, FontPixelShader()));
		SetGeometryShader(NULL);
    }
}