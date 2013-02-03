#include <Render\Direct3D10\SCTInputLayoutD3D10.h>
#include <Resource\Direct3D10\SCTShaderD3D10Lighting.h>


namespace SCT
{
namespace Resource
{

SCTShaderD3D10Lighting::SCTShaderD3D10Lighting(ID3D10Device *device)
:	mpWorldMatrix (NULL),
	mpViewMatrix (NULL),
	mpProjectionMatrix (NULL),
	mpCameraPosition (NULL),
	mpTexturePtr (NULL),
	mpMaterial (NULL)
{
	mpDevice	= device;
}

SCTShaderD3D10Lighting::~SCTShaderD3D10Lighting()
{
	Shutdown();
}

SCTReturn SCTShaderD3D10Lighting::Initialize(WCHAR* filename, char *techniqueName, Render::SCTVertexType type)
{
	SCTShaderD3D10::Initialize(filename, techniqueName, type);
	
	// Initialize the shader variables
	mpWorldMatrix		= mpShaderEffect->GetVariableByName("gWorld")->AsMatrix();
	mpViewMatrix		= mpShaderEffect->GetVariableByName("gView")->AsMatrix();
	mpProjectionMatrix	= mpShaderEffect->GetVariableByName("gProjection")->AsMatrix();
	mpCameraPosition	= mpShaderEffect->GetVariableByName("gCameraPosition")->AsVector();
	mpTexturePtr		= mpShaderEffect->GetVariableByName("gTextures")->AsShaderResource();

	mpMaterial			= mpShaderEffect->GetVariableByName("gMaterial");

	// Fog
	mpFogColour			= mpShaderEffect->GetVariableByName("gFogColour")->AsVector();
	mpFogStart			= mpShaderEffect->GetVariableByName("gFogStart")->AsScalar();
	mpFogEnd			= mpShaderEffect->GetVariableByName("gFogEnd")->AsScalar();

	// Debug
	mpRenderTextures	= mpShaderEffect->GetVariableByName("gbRenderTextures")->AsScalar();

	return OK;
}
		  
SCTReturn SCTShaderD3D10Lighting::Render()
{
	SCTShaderD3D10::Render();

	return OK;
}
		  
SCTReturn SCTShaderD3D10Lighting::Shutdown()
{
	SCTShaderD3D10::Shutdown();

	mpWorldMatrix		= NULL;
	mpViewMatrix		= NULL;
	mpProjectionMatrix	= NULL;
	mpCameraPosition	= NULL;

	return OK;
}

}	// End of Namespace Resource
}	// End of Namespace SCT