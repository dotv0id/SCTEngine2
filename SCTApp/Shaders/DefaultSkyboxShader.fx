
// Global variables
matrix gWorld;
matrix gView;
matrix gProjection;

// Nonnumeric values cannot be added to a cbuffer.
TextureCube gCubeMap;

SamplerState gTriLinearSample
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

RasterizerState rsNoCulling
{
    CullMode = None;
};

DepthStencilState LessEqualDSS
{
	// Make sure the depth function is LESS_EQUAL and not just LESS.  
	// Otherwise, the normalized depth values at z = 1 (NDC) will 
	// fail the depth test if the depth buffer was cleared to 1.
    DepthFunc = LESS_EQUAL;
};

struct VertexInput
{
	float3 position : POSITION;
};

struct PixelInput
{
	float4 position : SV_POSITION;
    float3 tex		: TEXCOORD;
};
 

PixelInput SkyboxVertexShader(VertexInput vIn)
{
	PixelInput vOut;
	
	matrix WVP	= mul(gWorld, gView);
	WVP	= mul(WVP, gProjection);

	// set z = w so that z/w = 1 (i.e., skydome always on far plane).
	vOut.position = mul(float4(vIn.position, 1.0f), WVP).xyww;
	
	// use local vertex position as cubemap lookup vector.
	vOut.tex = vIn.position;
	
	return vOut;
}

float4 SkyboxPixelShader(PixelInput pIn) : SV_Target
{
	return gCubeMap.Sample(gTriLinearSample, pIn.tex);
}


technique10 SkyTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, SkyboxVertexShader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, SkyboxPixelShader()));
        
        SetRasterizerState(rsNoCulling);
        SetDepthStencilState(LessEqualDSS, 0);
    }
}
