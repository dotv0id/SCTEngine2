// -------------------- +
//		Includes
// -------------------- +

#include <Core\SCTLogManager.h>
#include <Core\SCTStringHelper.h>
#include <Core\SCTWindows.h>


namespace SCT
{
namespace Core
{

SCTLogManager::SCTLogManager()
{
	// initialize error handling class...
}

SCTLogManager::~SCTLogManager()
{
}

SCTReturn SCTLogManager::Initialize()
{
	return OK;
}

SCTReturn SCTLogManager::Shutdown()
{
	return OK;
}

void SCTLogManager::displayError(SCTReturn lError)
{
	switch(lError)
		{
			// WIN32 Error Handling
			case REGISTER_CLASS_FAILED:
			{
				PrintDebug("Failed to register the WNDCLASSEX struct. [RegisterClassEx() FAILED]");
				MessageBox(0, L"RegisterClassEx() FAILED!", L"Error creating window", MB_OK);
			}

			case RENDER_WINDOW_CREATION_FAILED:
			{
				PrintDebug("Failed to create the render window. [CreateWindowEx() FAILED]");
				MessageBox(0, L"CreateWindowEx() FAILED!", L"Error creating window", MB_OK);
			}

			case CONFIG_DIALOG_CREATION_FAILED:
			{
				PrintDebug("Failed to create the configuration dialog. [CreateDialogParam() FAILED]");
				MessageBox(0, L"CreateDialogParam() FAILED!", L"Error creating configuration dialog", MB_OK);
			}
		}
}

// Displays DirectX errors
void SCTLogManager::displayError(HRESULT hr)
{
	if(hr)
	{
		std::wstring lDXError(DXGetErrorString(hr));
		std::wstring lDXErrorDesc(DXGetErrorDescription(hr));
	
		std::stringstream lErrorStr;

		lErrorStr	<< "Error: " << SCTStringHelper::getSingleton().wStringToString(lDXError)
					<< " error description: " << SCTStringHelper::getSingleton().wStringToString(lDXErrorDesc);

		PrintDebug(const_cast<char*>(lErrorStr.str().c_str()));
	}
}

void SCTLogManager::showDebugConsole()
{
	// SHOW THE DEBUG CONSOLE ONLY IN DEBUG VERSIONS
	//#if defined(DEBUG) || defined(_DEBUG)
	if(AllocConsole())
	{
		FILE *stream;
		//freopen("CONOUT$", "wt", stdout);
		freopen_s(&stream, "CONOUT$", "wt", stdout);

		SetConsoleTitle(L"SCT Game Engine: Debug Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout<<"*** SCT Game Engine: DEBUG Mode.\n";
	}
	//#endif
}

void SCTLogManager::hideDebugConsole()
{
	FreeConsole();
}

void  SCTLogManager::PrintDebug(const char* format, ...)
{
	//#if defined(DEBUG) || defined(_DEBUG)
		std::cout<< "> ";
		va_list argptr;
		va_start(argptr, format);
		vprintf(format, argptr);
		va_end(argptr);

		std::cout<< "\n";
	//#endif
}

void  SCTLogManager::coutSCTConfig(const SCTConfigRender &inConf)
{
	#ifdef _DEBUG	// OUTPUT TO THE DEBUG CONSOLE
	std::cout<<"\n*** Dumping lTmpConfig SCTConfiguration struct...\n";
	std::cout<<"*** Render Subsystem ***"<<"\n";
	std::cout<<inConf.RenderSubsystem<<"\n";

	std::cout<<"*** Render Device ***"<<"\n";
	std::cout<<inConf.RenderDevice<<"\n";
	
	std::cout<<"*** Resolution ***"<<"\n";
	std::cout<<"width:        "<<inConf.Resolution.width<<"\n";
	std::cout<<"height:	      "<<inConf.Resolution.height<<"\n";				
	std::cout<<"refresh rate: "<<inConf.Resolution.refreshRate<<"\n";

	std::cout<<"*** Fullscreen ***"<<"\n";
	std::cout<<inConf.Fullscreen<<"\n";

	std::cout<<"*** VSync ***"<<"\n";
	std::cout<<inConf.Vsync<<"\n\n";
	#endif // END OF DEBUG CONSOLE
}

void SCTLogManager::PrintShaderMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader_error_log.txt");

	std::cout << "> ";
	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
		std::cout << compileErrors[i];
	}
	std::cout << "\n";

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(Core::SCTWindows::getSingleton().GetRenderWindow(), L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

}	// End of Core	namespace
}	// End of SCT	namespace