#include <Render\Direct3D10\SCTRenderTextureD3D10.h>
#include <Resource\SCTTexture.h>
#include <Resource\Direct3D10\SCTTextureD3D10.h>


namespace SCT
{
namespace Render
{

SCTRenderTextureD3D10::SCTRenderTextureD3D10(ID3D10Device *device)
:	mpDevice (device),
	mpRenderTargetView (NULL),
	mpRenderTargetTexture (NULL)
{
}

SCTRenderTextureD3D10::~SCTRenderTextureD3D10()
{
	Shutdown();
}

SCTReturn SCTRenderTextureD3D10::Initialize(SUInt32 width, SUInt32 height)
{
	HRESULT hr;
	SCTReturn	sResult;

	mpRenderTargetTexture	= new Resource::SCTTextureD3D10(mpDevice);
	
	sResult	= mpRenderTargetTexture->Initialize(width, height, D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE);

	if(sResult != OK)
		return sResult;

	// Setup the description of the render target view
	D3D10_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

	renderTargetViewDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	renderTargetViewDesc.ViewDimension		= D3D10_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	hr = mpDevice->CreateRenderTargetView(mpRenderTargetTexture->mDXTexture, &renderTargetViewDesc, &mpRenderTargetView);
	if(hr != S_OK)
		return FAILED_TO_CREATE_RENDER_TARGET_TEXTURE;

	return OK;
}

SCTReturn SCTRenderTextureD3D10::Shutdown()
{
	// Release the Target View
	ReleaseCOM(mpRenderTargetView);

	// Release the texture
	delete mpRenderTargetTexture;
	mpRenderTargetTexture	= NULL;

	return OK;
}

}	// End of Render Namespace
}	// End of SCT Namespace