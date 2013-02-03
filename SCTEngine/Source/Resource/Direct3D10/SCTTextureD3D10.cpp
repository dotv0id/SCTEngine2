#include <Resource\Direct3D10\SCTTextureD3D10.h>


namespace SCT
{
namespace Resource
{

SCTTextureD3D10::SCTTextureD3D10(ID3D10Device *device)
:	mpDevice (device),
	mDXTexture (NULL), 
	mpShaderResourceView (NULL)
{
}

SCTTextureD3D10::~SCTTextureD3D10()
{
	Shutdown();
}
			
SCTReturn SCTTextureD3D10::Initialize(const WCHAR *filename, bool asCubeMap)
{
	// Load the texture
	HRESULT result;
	
	if(asCubeMap)
	{
		D3DX10_IMAGE_LOAD_INFO loadInfo;
		loadInfo.MiscFlags = D3D10_RESOURCE_MISC_TEXTURECUBE;

		ID3D10Texture2D* tex = 0;
		result = D3DX10CreateTextureFromFile(mpDevice, filename, &loadInfo, 0, (ID3D10Resource**)&tex, 0);

		D3D10_TEXTURE2D_DESC texDesc;
		tex->GetDesc(&texDesc);

		D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
		viewDesc.Format							= texDesc.Format;
		viewDesc.ViewDimension					= D3D10_SRV_DIMENSION_TEXTURECUBE;
		viewDesc.TextureCube.MipLevels			= texDesc.MipLevels;
		viewDesc.TextureCube.MostDetailedMip	= 0;
    
		result = mpDevice->CreateShaderResourceView(tex, &viewDesc, &mpShaderResourceView);
   
		ReleaseCOM(tex);
	}
	else
	{
		result = D3DX10CreateShaderResourceViewFromFile(mpDevice, filename, NULL, NULL, &mpShaderResourceView, NULL);
	}

	if(result != S_OK)
			return FAIL;

	return OK;
}

SCTReturn SCTTextureD3D10::Initialize(ID3D10Texture2D* depthStencil)
{
	HRESULT hr;

	D3D10_SHADER_RESOURCE_VIEW_DESC     shaderResourceViewDesc;

	shaderResourceViewDesc.Format						= DXGI_FORMAT_R32_FLOAT;
    shaderResourceViewDesc.ViewDimension				= D3D10_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip	= 0;
    shaderResourceViewDesc.Texture2D.MipLevels			= 1;

	// Create the shader resource view for the depth buffer
	hr = mpDevice->CreateShaderResourceView(depthStencil, &shaderResourceViewDesc, &mpShaderResourceView);
	if(hr != S_OK)
		return FAILED_TO_CREATE_TEXTURE;

	return OK;
}

SCTReturn SCTTextureD3D10::Initialize(SUInt32 width, SUInt32 height, SUInt32 flags)
{
	// Creates a texture resource of custom size
	// Currently used for SCTRenderTexture render target
	HRESULT hr;

	D3D10_TEXTURE2D_DESC			textureDesc;
	D3D10_SHADER_RESOURCE_VIEW_DESC	shaderResourceViewDesc;

	// Initialize the render target texture description
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the texture description
	textureDesc.Width				= width;
	textureDesc.Height				= height;
	textureDesc.MipLevels			= 1;
	textureDesc.ArraySize			= 1;
	textureDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count	= 1;
	textureDesc.Usage				= D3D10_USAGE_DEFAULT;
	textureDesc.BindFlags			= D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;//flags;
	textureDesc.CPUAccessFlags		= 0;
    textureDesc.MiscFlags			= 0;

	// Create the render target texture
	hr = mpDevice->CreateTexture2D(&textureDesc, NULL, &mDXTexture);
	if(hr != S_OK)
		return FAILED_TO_CREATE_TEXTURE;


	// Setup the description of the shader resource view
	shaderResourceViewDesc.Format						= textureDesc.Format;
	shaderResourceViewDesc.ViewDimension				= D3D10_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip	= 0;
	shaderResourceViewDesc.Texture2D.MipLevels			= 1;

	// Create the shader resource view
	hr = mpDevice->CreateShaderResourceView(mDXTexture, &shaderResourceViewDesc, &mpShaderResourceView);
	if(hr != S_OK)
		return FAILED_TO_CREATE_TEXTURE;

	return OK;
}

SCTReturn SCTTextureD3D10::Shutdown()
{
	// Release the ShaderResourceView
	ReleaseCOM(mpShaderResourceView);

	// Release the texture Dx10 resource
	ReleaseCOM(mDXTexture);

	return OK;
}

}	// End of Namespace Resource
}	// End of Namespace SCT