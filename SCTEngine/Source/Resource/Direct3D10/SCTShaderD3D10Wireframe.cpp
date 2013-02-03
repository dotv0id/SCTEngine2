#include <Render\Direct3D10\SCTInputLayoutD3D10.h>
#include <Resource\Direct3D10\SCTShaderD3D10Wireframe.h>


namespace SCT
{
namespace Resource
{

SCTShaderD3D10Wireframe::SCTShaderD3D10Wireframe(ID3D10Device *device)
{
	mpDevice	= device;
}

SCTShaderD3D10Wireframe::~SCTShaderD3D10Wireframe()
{
	Shutdown();
}

SCTReturn SCTShaderD3D10Wireframe::Initialize(WCHAR* filename, char *techniqueName, Render::SCTVertexType type)
{		 
	SCTShaderD3D10::Initialize(filename, techniqueName, type);

	// Initialize the shader variables
	mpWVPMatrix			= mpShaderEffect->GetVariableByName("gWVP")->AsMatrix();
	mpWireframeColor	= mpShaderEffect->GetVariableByName("gWireframeColor")->AsVector();

	return OK;
}		  
		  
SCTReturn SCTShaderD3D10Wireframe::Render()
{		  
	return OK;
}		  
		  
SCTReturn SCTShaderD3D10Wireframe::Shutdown()
{
	mpWVPMatrix			= NULL;
	mpWireframeColor	= NULL;

	return OK;
}

}	// End of Namespace Resource
}	// End of Namespace SCT