#include <Resource\Direct3D10\SCTShaderD3D10.h>
#include <Render\Direct3D10\SCTInputLayoutD3D10.h>

#include <SCTCore.h>


namespace SCT
{
namespace Resource
{

SCTShaderD3D10::SCTShaderD3D10(ID3D10Device *device)
:	mpShaderEffect (NULL),
	mpTechnique (NULL),
	mpInputLayout (NULL)
{
	// Get the Direct3D device
	mpDevice	= device;
}

SCTShaderD3D10::~SCTShaderD3D10()
{
	Shutdown();
}

SCTReturn SCTShaderD3D10::Initialize(WCHAR* filename, char *techniqueName, Render::SCTVertexType type)
{
	HRESULT						result;
	ID3D10Blob					*errorMessage;

	// Initialize the error message.
	errorMessage = 0;

	// Load the shader in from the file.
	result = D3DX10CreateEffectFromFile(filename, NULL, NULL, "fx_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 
										mpDevice, NULL, NULL, &mpShaderEffect, &errorMessage, NULL);
	if(result != S_OK)
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			Core::SCTLogManager::getSingleton().PrintShaderMessage(errorMessage, filename);
		}
		else
		{
			// If there was  nothing in the error message then it simply could not find the shader file itself.
			Core::SCTLogManager::getSingleton().PrintDebug(	"Missing Shader file '%s'!", 
															Core::SCTStringHelper::getSingleton().wStringToString(filename).c_str());
		}

		return FAIL;
	}


	mpTechnique		= mpShaderEffect->GetTechniqueByName(techniqueName);

	// Create the Input layout for this Shader
	mpInputLayout	= new Render::SCTInputLayoutD3D10(mpDevice, type, this, techniqueName);

	return OK;
}

SCTReturn SCTShaderD3D10::Render()
{
	return OK;
}

SCTReturn SCTShaderD3D10::Shutdown()
{
	ReleaseCOM(mpShaderEffect);
	
	mpTechnique	= NULL;

	delete mpInputLayout;
	mpInputLayout	= NULL;

	return OK;
}

}	// End of Namespace Resource
}	// End of Namespace SCT