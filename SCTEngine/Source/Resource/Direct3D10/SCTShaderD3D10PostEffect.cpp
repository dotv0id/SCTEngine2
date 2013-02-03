#include <Resource\Direct3D10\SCTShaderD3D10PostEffect.h>

namespace SCT
{

namespace Resource
{

SCTShaderD3D10PostEffect::SCTShaderD3D10PostEffect(ID3D10Device *device)
:	mpWVPMatrix (NULL),
	mpTexturePtr (NULL),
	mpDepthTexturePtr (NULL),
	mpVpWidth (NULL),
	mpVpHeight (NULL),
	mpProjectionMatrix (NULL)
{
	mpDevice	= device;
}

SCTShaderD3D10PostEffect::~SCTShaderD3D10PostEffect()
{
	Shutdown();
}

SCTReturn SCTShaderD3D10PostEffect::Initialize(WCHAR* filename, char *techniqueName, Render::SCTVertexType type)
{
	SCTShaderD3D10::Initialize(filename, techniqueName, type);

	// Initialize the shader variables
	mpWVPMatrix				= mpShaderEffect->GetVariableByName("gWVP")->AsMatrix();
	mpProjectionMatrix		= mpShaderEffect->GetVariableByName("gProjection")->AsMatrix();
	mpTexturePtr			= mpShaderEffect->GetVariableByName("gPETexture")->AsShaderResource();
	mpDepthTexturePtr		= mpShaderEffect->GetVariableByName("gDepthTexture")->AsShaderResource();
	mpDoFParams				= mpShaderEffect->GetVariableByName("gDoFParams")->AsVector();

	mpDownscaleResolution	= mpShaderEffect->GetVariableByName("gDownscaleResolution")->AsVector();;

	mpVpWidth				= mpShaderEffect->GetVariableByName("gViewportInvWidth")->AsScalar();
	mpVpHeight				= mpShaderEffect->GetVariableByName("gViewportInvWidth")->AsScalar();

	return OK;
}

SCTReturn SCTShaderD3D10PostEffect::Render()
{
	SCTShaderD3D10::Render();

	return OK;
}

SCTReturn SCTShaderD3D10PostEffect::Shutdown()
{
	SCTShaderD3D10::Shutdown();

	mpWVPMatrix				= NULL;
	mpTexturePtr			= NULL;
	mpVpWidth				= NULL;
	mpVpHeight				= NULL;
	mpDownscaleResolution	= NULL;

	return OK;
}

}	// End of Namespace Resource
}	// End of Namespace SCT