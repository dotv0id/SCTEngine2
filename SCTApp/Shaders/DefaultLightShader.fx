//! ------------------------------------------------------------
//	@File:			DefaultLightShader.fx
//   
//	Author: Anastasios 'v0id' Giannakopoulos
//	E-Mail: dotvoidd@gmail.com
//   
//	Description:	Default Shader used to render wireframe
// ------------------------------------------------------------


#include "ShaderCommon.fxh"


// --- Global variables ---
uint		MAX_LIGHTS			= 3;	// Maximum number of lights supported in the scene
float		gAmbientIntensity	= 0.25;	// Change dynamically

// Matrices
matrix		gWorld;						// World matrix of the object to render
matrix		gView;
matrix		gProjection;
float3		gCameraPosition;			// Camera position: used for lighting calculations

Material	gMaterial;

bool		gbRenderTextures;
Texture2D	gTextures[MAX_TEXTURES];	// textures are mapped in the following order:
										// Ambient, Diffuse, Specular, Alpha, Bump


// -- Input, Output structures for the shader programs ---
// Vertex Input type
struct VertexInput
{
	float3 position : POSITION;
    float2 tex		: TEXCOORD0;
	float3 normal	: NORMAL;
};

struct VertexInputPTBN
{
	float3 position : POSITION;
	float3 tangent	: TANGENT;
	float3 binormal	: BINORMAL;
	float3 normal	: NORMAL;
	float2 tex		: TEXCOORD0;
};

// Pixel Input type
struct PixelInput
{
	float4 position : SV_POSITION;
    float2 tex		: TEXCOORD0;
	float2 texDepth	: TEXCOORD1;
   	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float3 binormal	: BINORMAL;
	float fogFactor : FOG;
};


// --- Lighting functions ---

// Phong Lighting Reflection Model
float4 calcPhongLighting(Material material, float4 LColor, float3 N, float3 L, float3 V, float3 R)
{
	float4 Ia = material.Ka * gAmbientIntensity;
	float4 Id = material.Kd * saturate( dot(N,L) );
	float4 Is = 0;//material.Ks * pow(saturate(dot(R,V)), M.A);

	return Ia + (Id + Is) * LColor;
}

// Blinn-Phong Lighting Reflection Model
float4 calcBlinnPhongLighting(Material material, float4 LColor, float3 N, float3 L, float3 H, Textures tex)
{
	float4 Ia, Id, Is;

	Ia		= material.Ka * gAmbientIntensity;
	Id		= material.Kd * saturate(dot(N, L)); //tex.diffuseTexture;
	Is		= 0;//material.Ks * pow(saturate(dot(N,H)), material.mShininess);

	Ia.w	= 1.0f;
	Id.w	= 1.0f;
	Is.w	= 1.0f;

	if(gbRenderTextures)
	{
		Ia *= tex.ambientTexture;
		Id *= tex.diffuseTexture;
		Is *= tex.specularTexture;
	}

	return (Ia + (Id + Is) * LColor);
}


// --- Shader functions ---
// Vertex Shader
PixelInput LightVertexShader(VertexInputPTBN input)
{
	PixelInput	output;
	float4		cameraPosition;

	Matrix		WV	= mul(gWorld, gView);
	Matrix		WVP	= mul(gWorld, gView);
	WVP	 = mul(WVP, gProjection);

	// Calculate the position of the vertex against the world, view, and projection matrices
    output.position = mul(float4(input.position, 1.0f), WVP);

	// Store the texture coordinates for the pixel shader
    output.tex		= input.tex;
    
	// Calculate the normals, tangents, binormals against the world matrix
	output.normal	= normalize(mul(input.normal, (float3x3)gWorld));
	output.tangent	= normalize(mul(input.tangent, (float3x3)gWorld));
	output.binormal	= normalize(mul(input.binormal, (float3x3)gWorld));

	// Fog calculation
	cameraPosition = mul(float4(input.position, 1.0f), WV);

    // Calculate linear fog.    
	output.fogFactor = saturate((gFogEnd - cameraPosition.z) / (gFogEnd - gFogStart));

	return output;
}

// Pixel Shader
float4 LightPixelShader(PixelInput input, uniform bool bLight) : SV_Target
{
	input.normal	= normalize(input.normal);
	
	Light light;
	light.dir	= float3(-1.0f, -1.0f, -1.0f);
	light.dir	= normalize(light.dir);
	light.color	= float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// Sample the texture pixel at this location.	
	float3 V	= normalize(gCameraPosition - (float3)input.position);
	float3 R	= reflect(-light.dir, (float3)input.normal);
	float3 H	= normalize(-light.dir + V);
	
	// Texture color
	Textures texColor;
	texColor.ambientTexture		= gTextures[AMBIENT].Sample(SampleType, input.tex);
	texColor.diffuseTexture		= gTextures[DIFFUSE].Sample(SampleType, input.tex);
	texColor.specularTexture	= gTextures[SPECULAR].Sample(SampleType, input.tex);
	texColor.alphaTexture		= gTextures[ALPHA].Sample(SampleType, input.tex);
	
	clip(texColor.diffuseTexture.a - 0.55f);

	// Calculate Bump map
	float4 bumpTexture			= gTextures[BUMP].Sample(SampleType, input.tex);

	// Expand the range of the normal value from (0, +1) to (-1, +1).
	bumpTexture					= (bumpTexture * 2.0f) - 1.0f;

	// Calculate bump normal
	float3 bumpNormal = float3(0, 0, 0);
	bumpNormal				+= input.normal + bumpTexture.x * input.tangent + bumpTexture.y * input.binormal;
	bumpNormal				= normalize(bumpNormal);

	//return float4(input.binormal, 1.0f);

	//return float4(bumpNormal, 1.0f);

	// Claculate light intensity
	float4 I		= calcBlinnPhongLighting(gMaterial, light.color, bumpNormal, -light.dir, H, texColor);

	// Debug Textures
	float4 outColor;
	if(bLight)
		outColor = input.fogFactor * I + (1.0f - input.fogFactor) * gFogColour;
	//else
		//outColor = input.fogFactor * texColor.diffuseTexture + (1.0f - input.fogFactor) * gFogColour;
	
	return outColor;
}

// Render depth buffer (taken from directx 10 samples)
PixelInput VertexShaderDepth(VertexInputPTBN input)
{
	PixelInput	output;
	
	Matrix WV	= mul(gWorld, gView);
	Matrix WVP	= mul(gWorld, gView);
	WVP			= mul(WVP, gProjection);

	output.position = mul(float4(input.position, 1.0), WVP);

	output.tex		= input.tex;
	output.texDepth = output.position.zw;
	
	return output;
}

float4 PixelShaderDepth(PixelInput input) : SV_Target
{
	// Need to find a way to render the alpha to the backbuffer
	float fDepth;// = input.texDepth.x / input.texDepth.y;

	float4 diffuse = gTextures[DIFFUSE].Sample(SampleType, input.tex);
	
	//if(diffuse.w > 0)
	fDepth = input.texDepth.x / input.texDepth.y;

	return float4(fDepth.xxx, diffuse.w);
}

// ------------ Techniques -------------
// -------------------------------------
technique10 LightNoAlpha
{
    pass pass0
    {
		SetVertexShader(CompileShader(vs_4_0, LightVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, LightPixelShader(true)));

		SetRasterizerState(rsSolidCullingNone);
		SetBlendState(bsAlphaDisabled, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(dsDepthTestWriteEnabled, 0);
    }
}

technique10 LightAlphaEnabled
{
	// Cull back, shade back
    pass pass0
    {
		SetVertexShader(CompileShader(vs_4_0, LightVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, LightPixelShader(true)));

		SetRasterizerState(rsSolidCullingNone);
		SetBlendState(bsAlphaEnabled, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(dsDepthTestWriteEnabled, 0);
    }
}

technique10 RenderDepth
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, VertexShaderDepth()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PixelShaderDepth()));
        
		SetRasterizerState(rsSolidCullingNone);
        SetBlendState(bsAlphaEnabled, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF);
        SetDepthStencilState(dsDepthTestWriteEnabled, 0);
	}
}