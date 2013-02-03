#include <Resource\Direct3D10\SCTShaderD3D10SkyBox.h>

namespace SCT
{
namespace Resource
{

SCTShaderD3D10SkyBox::SCTShaderD3D10SkyBox(ID3D10Device *device)
:	mpWorldMatrix (NULL),
	mpViewMatrix (NULL),
	mpProjectionMatrix (NULL),
	mpTexturePtr (NULL)
{
	mpDevice	= device;
}

SCTShaderD3D10SkyBox::~SCTShaderD3D10SkyBox()
{
	Shutdown();
}

SCTReturn SCTShaderD3D10SkyBox::Initialize(WCHAR* filename, char *techniqueName, Render::SCTVertexType type)
{
	SCTShaderD3D10::Initialize(filename, techniqueName, type);
	
	// Initialize the shader variables
	mpWorldMatrix		= mpShaderEffect->GetVariableByName("gWorld")->AsMatrix();
	mpViewMatrix		= mpShaderEffect->GetVariableByName("gView")->AsMatrix();
	mpProjectionMatrix	= mpShaderEffect->GetVariableByName("gProjection")->AsMatrix();

	mpTexturePtr		= mpShaderEffect->GetVariableByName("gCubeMap")->AsShaderResource();

	return OK;
}

SCTReturn SCTShaderD3D10SkyBox::Render()
{
	SCTShaderD3D10::Render();

	return OK;
}

SCTReturn SCTShaderD3D10SkyBox::Shutdown()
{
	SCTShaderD3D10::Shutdown();

	mpWorldMatrix		= NULL;
	mpViewMatrix		= NULL;
	mpProjectionMatrix	= NULL;

	return OK;
}

}	// End of Resource Namespace
}	// End of SCT Namespace