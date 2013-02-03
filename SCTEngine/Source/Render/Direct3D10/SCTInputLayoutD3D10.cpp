#include <SCTCore.h>
#include <Render\Direct3D10\SCTInputLayoutD3D10.h>
#include <Resource\Direct3D10\SCTShaderD3D10.h>


namespace SCT
{
namespace Render
{

SCTInputLayoutD3D10::SCTInputLayoutD3D10(ID3D10Device *device, SCTVertexType type, Resource::SCTShaderD3D10 *shader, char *techniqueName)
{
	// Get the Direct3D device
	mpDevice			= device;
	SUInt32	numElements	= 0;

	// --- Declare Input Layouts ---
	// A. Input Layout Desc Position
	static D3D10_INPUT_ELEMENT_DESC D3D10PosVertexDesc[1] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	// B. Input Layout Desc Position Color
	static D3D10_INPUT_ELEMENT_DESC D3D10PosColorVertexDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,								D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	// C. Input Layout Desc Position Normal Texture
	static D3D10_INPUT_ELEMENT_DESC D3D10PosNormTexVertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,							  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	// D. Input Layout Desc Position Texture
	static D3D10_INPUT_ELEMENT_DESC D3D10PosTextureVertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,							  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	// E. Input Layout Desc Position Tangent Normal Texture
	static D3D10_INPUT_ELEMENT_DESC D3D1PosTangNormTexVertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,							  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	// F. Input Layout Desc Position Tangent Binormal Normal Texture
	static D3D10_INPUT_ELEMENT_DESC D3D1PosTangBiNormTexVertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,							  D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	// Choose the InputLayout
	D3D10_PASS_DESC				PassDesc;
	D3D10_INPUT_ELEMENT_DESC	*currentVertexDesc;
	char						*typeString;
	
	switch(type)
	{
	case ePosition:
		{
			currentVertexDesc	= D3D10PosVertexDesc;
			numElements			= 1;
			typeString			= "Position";
		} break;

	case ePosColor:
		{
			currentVertexDesc	= D3D10PosColorVertexDesc;
			numElements			= 2;
			typeString			= "Position Color";
		} break;

	case ePosTexture:
		{
			currentVertexDesc	= D3D10PosTextureVertexDesc;
			numElements			= 2;
			typeString			= "Position Texture";
		} break;

	case ePosNormTex:
		{
			currentVertexDesc	= D3D10PosNormTexVertexDesc;
			numElements			= 3;
			typeString			= "Position Normal Texture";
		} break;

	case ePosTangNormTex:
		{
			currentVertexDesc	= D3D1PosTangNormTexVertexDesc;
			numElements			= 4;
			typeString			= "Position Tangent Normal Texture";
		} break;

	case ePosTangBiNormTex:
		{
			currentVertexDesc	= D3D1PosTangBiNormTexVertexDesc;
			numElements			= 5;
			typeString			= "Position Tangent Binormal Normal Texture";
		} break;
	}
	
		
	// Create the input Layout
	shader->mpShaderEffect->GetTechniqueByName(techniqueName)->GetPassByIndex(0)->GetDesc(&PassDesc);
	HRESULT res = mpDevice->CreateInputLayout(currentVertexDesc, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &mpDXInputLayout);
	if(res != S_OK)
	{
		Core::SCTLogManager::getSingleton().PrintDebug("Error Creating Input layout of type [%s]", typeString);
	}
}
		
SCTInputLayoutD3D10::~SCTInputLayoutD3D10()
{
	Shutdown();
}

SCTReturn SCTInputLayoutD3D10::Shutdown()
{
	ReleaseCOM(mpDXInputLayout);
	
	return OK;
}

}	// End of Namesace Render
}	// End of Namespace SCT