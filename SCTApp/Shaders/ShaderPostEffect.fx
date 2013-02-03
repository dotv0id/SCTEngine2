//! ------------------------------------------------------------
//	@File:			ShaderPostEffect.fx
//   
//	Author: Anastasios 'v0id' Giannakopoulos
//	E-Mail: dotvoidd@gmail.com
//   
//	Description:	Test shader for posteffects
// ------------------------------------------------------------

#include "ShaderPostEffectCommon.fxh"

// Global variables
matrix		gWVP;
matrix		gProjection;

Texture2D	gPETexture;
Texture2D	gDepthTexture;
float		gViewportInvWidth;
float		gViewportInvHeight;
float2		gDownscaleResolution;
float4		gDoFParams;

// Offsets used to downscale an image
static const float gOffsets[4] = {-1.5f, -0.5f, 0.5f, 1.5f};


// Vertex Shader Input type
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex		: TEXCOORD0;
};

// Pixel Shader Input type
struct PixelInputType
{
    float4 position	: SV_POSITION;
    float2 tex		: TEXCOORD0;
};


/* -------------------------------------------------------------
		Post processing: Sprite shaders
   ------------------------------------------------------------- */
// Render Post processing sprite: Vertex shader
PixelInputType SpriteVertexShader(VertexInputType input)
{
	PixelInputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, gWVP);
	output.position.w = 1.0f;
	
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	return output;
}

// Render Post processing sprite: Pixel shader
float4 SpritePixelShader(PixelInputType input) : SV_Target
{
	return gPETexture.Sample(ssPoint, input.tex);
}


/* -------------------------------------------------------------
		Post processing: Downscale / Upscale shaders
   ------------------------------------------------------------- */
// Downscales to 1/16 size, using 16 samples
float4 DownscalePixelShader(PixelInputType input, uniform bool bDecodeLuminance) : SV_Target
{
	float4 color = 0;

	for(int x = 0; x < 4; x++)
	{
		for(int y = 0; y < 4; y++)
		{
			float2 offset;

			offset				= float2(gOffsets[x], gOffsets[y]) / gDownscaleResolution;
			float4 texSample	= gPETexture.Sample(ssPoint, input.tex + offset);
			
			color += texSample;
		}
	}
	
	color /= 16.0f;
	
	color.w = 0.5f;

	return color;
}

// Upscales or downscales using hardware bilinear filtering
float4 ScaleHWPixelShader(PixelInputType input) : SV_Target
{
	return gPETexture.Sample(ssLinear, input.tex);
}


/* -------------------------------------------------------------
		Post processing: Gaussian blur shaders
   ------------------------------------------------------------- */
float4 GaussianBlurH(PixelInputType input, uniform int radius) : SV_Target
{
	float4	vColor		= float4(0, 0, 0, 0);
	float2	texCoord	= input.tex;

	float texelSizeH	= 1.0f / 600;

	// Use this pixel's depth value to weight the blur
	float	fDepth;
	int2	iScreenCoord = int2(input.tex * gDoFParams.yz);
	fDepth	= gDepthTexture.Load( int3(iScreenCoord, 0));

    for (int i = -radius; i < radius; i++)
    {   
		float fWeight			= CalcGaussianWeight(i, 1.0f * fDepth);

		float2 neighborTexCoord	= texCoord + float2(texelSizeH * i, 0);

		float4 vSample	= gPETexture.Sample(ssPoint, neighborTexCoord);
		vColor += vSample * fWeight;
    }
	
	return vColor;
}

float4 GaussianBlurW(PixelInputType input, uniform int radius) : SV_Target
{
	float4	vColor		= float4(0, 0, 0, 0);
	float2	texCoord	= input.tex;

	float texelSizeW	= 1.0f / 800;

	// Use this pixel's depth value to weight the blur
	float	fDepth;
	int2	iScreenCoord = int2(input.tex * gDoFParams.yz);
	fDepth	= gDepthTexture.Load( int3(iScreenCoord, 0));

    for (int i = -radius; i < radius; i++)
    {   
		float fWeight			= CalcGaussianWeight(i, 1.0f * fDepth);

		float2 neighborTexCoord	= texCoord + float2(0, texelSizeW * i);

		float4 vSample	= gPETexture.Sample(ssPoint, neighborTexCoord);
		vColor += vSample * fWeight;
    }

	return vColor;
}

/* -------------------------------------------------------------
		Post processing: Depth of field post effect
   ------------------------------------------------------------- */
float4 DepthOfFieldPixelShader(PixelInputType input) : SV_Target
{
	int2 iScreenCoord = int2(input.tex * gDoFParams.yz);
	
	// Get depth information
	float fDepth;
	fDepth = gDepthTexture.Load( int3(iScreenCoord, 0));

	// test
	float	nearDist	= 15.0f;
	float	nearRange	= 20.0f;
	float	farDist		= 20.0f;
	float	farRange	= 30.0f;
	
	float4 fDepthSample = mul(float4( input.tex, fDepth, 1), gProjection);
	fDepth = fDepthSample.z / fDepthSample.w;

	float fBlurFactor	= clamp(1, 0, (fDepth - nearDist) / nearRange);
	
	float4 final		= gPETexture.Sample(ssPoint, input.tex);
	//final.w = 1.0f;
	final.w				= fBlurFactor;		// Set alpha according to the depth buffer

	return final;
}

/* -------------------------------------------------------------
		Post processing: Gaussian blur shaders
   ------------------------------------------------------------- */
// Render quad, no Post effect
technique10 RenderQuad
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, SpriteVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, SpritePixelShader()));

		SetRasterizerState(rsCullingBack);
		SetBlendState(bsAlphaDisabled, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(dsDepthTestWriteDisabled, 0);
	}
}

technique10 RenderQuadAlpha
{
pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, SpriteVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, SpritePixelShader()));

		SetRasterizerState(rsCullingBack);
		SetBlendState(bsAlphaEnabled, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(dsDepthTestWriteDisabled, 0);
	}
}

// Downscale
technique10 Downscale4
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, SpriteVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, DownscalePixelShader(false)));

		SetRasterizerState(rsCullingBack);
		SetBlendState(bsAlphaDisabled, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(dsDepthTestWriteDisabled, 0);
	}
}

// Gaussian Blur
technique10 GaussianBlurHorizontal
{
	// Gaussian blur vertical
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, SpriteVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, GaussianBlurW(6)));

		SetRasterizerState(rsCullingBack);
		SetBlendState(bsAlphaDisabled, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(dsDepthTestWriteDisabled, 0);
	}
}

technique10 GaussianBlurVertical
{
	// Gaussian blur vertical
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, SpriteVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, GaussianBlurH(6)));

		SetRasterizerState(rsCullingBack);
		SetBlendState(bsAlphaDisabled, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(dsDepthTestWriteDisabled, 0);
	}
}

// Depth of field
technique10 RenderDoF
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, SpriteVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, DepthOfFieldPixelShader()));

		SetRasterizerState(rsCullingBack);
		SetBlendState(bsAlphaEnabled, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(dsDepthTestWriteDisabled, 0);
	}
}