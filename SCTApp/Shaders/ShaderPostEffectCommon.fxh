//! ------------------------------------------------------------
//	@File:			ShaderPostEffectCommon.fx
//   
//	Author: Anastasios 'v0id' Giannakopoulos
//	E-Mail: dotvoidd@gmail.com
//   
//	Description:	Common functions and structures
//					used for post effects
// ------------------------------------------------------------


float CalcGaussianWeight(int iSamplePoint, float fSigma)
{
	float g = 1.0f / sqrt(2.0f * 3.14159 * fSigma * fSigma);  
	return (g * exp(-(iSamplePoint * iSamplePoint) / (2 * fSigma * fSigma)));
}


// States
// Sampler States
SamplerState ssPoint
{
    Filter      = MIN_MAG_MIP_POINT;
    AddressU    = Clamp;
    AddressV    = Clamp;
};

SamplerState ssLinear
{
	Filter      = MIN_MAG_POINT_MIP_LINEAR;
    AddressU    = Clamp;
    AddressV    = Clamp;
};

// Rasterizer State
RasterizerState rsCullingBack
{
	FillMode	= Solid;
	CullMode	= Back;
};

// Blending State
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

DepthStencilState dsDepthTestWriteDisabled
{
    DepthEnable		= FALSE;
    DepthWriteMask	= ZERO;
};


// Gaussian blur (W H) - technique, 2 passes
// Downscale & upscale - 2 techniques
// Render depth 1 technique