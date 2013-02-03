//! ------------------------------------------------------------
//	@File:			SCTShaderCommon.fxh
//   
//	Author: Anastasios 'v0id' Giannakopoulos
//	E-Mail: dotvoidd@gmail.com
//   
//	Description:	States, variables and defines used in the shaders
// ------------------------------------------------------------

// Texture arrays
#define		MAX_TEXTURES	5			// maximum number of textures
#define		AMBIENT			0
#define		DIFFUSE			1
#define		SPECULAR		2
#define		ALPHA			3
#define		BUMP			4

// Material properties
struct Material
{
	float4	Ka,
			Kd,
			Ks;

	float	mShininess,
			mShininessStrength;
};

// Surface texture arrays
struct Textures
{
	float4 ambientTexture;	
	float4 diffuseTexture;	
	float4 specularTexture;	
	float4 alphaTexture;
	float4 bumpTexture;
	float3 bumpNormal;
};

// Light properties
struct Light
{
	float4 color;
	float3 dir;
};

// Fog properties
bool		gFog;
float		gFogStart;
float		gFogEnd;
float4		gFogColour;



//	Render States
// -----------------------------
// Sampler states
SamplerState SampleType
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

SamplerState ssPoint
{
    Filter      = MIN_MAG_MIP_POINT;
    AddressU    = Clamp;
    AddressV    = Clamp;
};

// Rasterizer States
RasterizerState rsSolidCullingBack
{
	FillMode	= Solid;
	CullMode	= Back;
};

RasterizerState rsSolidCullingFront
{
	FillMode	= Solid;
	CullMode	= Front;
};

RasterizerState rsSolidCullingNone
{
	FillMode	= Solid;
	CullMode	= None;
};


// Blending States
BlendState bsAlphaDisabled
{
	BlendEnable[0] = FALSE;
};

BlendState bsAlphaEnabled
{
	AlphaToCoverageEnable       = FALSE;
    BlendEnable[0]              = TRUE;
    SrcBlend                    = SRC_ALPHA;
    DestBlend                   = INV_SRC_ALPHA;
    BlendOp                     = ADD;
    SrcBlendAlpha               = ZERO;
    DestBlendAlpha              = ZERO;
    BlendOpAlpha                = ADD;
    RenderTargetWriteMask[0]    = 0x0F;
};

// Depth stencil states
DepthStencilState dsDepthTestWriteEnabled
{
    DepthEnable                 = TRUE;
    DepthWriteMask              = ALL;
};

DepthStencilState dsDepthTestWriteDisabled
{
    DepthEnable                 = FALSE;
    DepthWriteMask              = ZERO;
};