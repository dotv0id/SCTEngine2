#include <Resource\Direct3D10\SCTShaderD3D10Font.h>
#include <Render\Direct3D10\SCTInputLayoutD3D10.h>

namespace SCT
{
namespace Resource
{

SCTShaderD3D10Font::SCTShaderD3D10Font(ID3D10Device *device)
:	mpWVPMatrix (NULL),
	mpTexturePtr (NULL),
	mpPixelColorPtr (NULL)
{
	// Get the Direct3D device
	mpDevice	= device;
}

SCTShaderD3D10Font::~SCTShaderD3D10Font()
{
	Shutdown();
}

SCTReturn SCTShaderD3D10Font::Initialize(WCHAR* filename, char *techniqueName, Render::SCTVertexType type)
{
	SCTShaderD3D10::Initialize(filename, techniqueName, type);

	// Initialize the shader variables
	mpWVPMatrix		= mpShaderEffect->GetVariableByName("gWVP")->AsMatrix();
	mpTexturePtr	= mpShaderEffect->GetVariableByName("gTexture")->AsShaderResource();
	mpPixelColorPtr = mpShaderEffect->GetVariableByName("gPixelColor")->AsVector();

	return OK;
}

SCTReturn SCTShaderD3D10Font::Render()
{
	return OK;
}

SCTReturn SCTShaderD3D10Font::Shutdown()
{
	mpWVPMatrix		= NULL;
	mpTexturePtr	= NULL;
	mpPixelColorPtr	= NULL;

	return OK;
}

}	// End of Namespace Resource
}	// End of Namespace SCT