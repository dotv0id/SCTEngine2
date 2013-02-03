#include <SCTCore.h>
#include <Input\Win32\SCTInputManagerWin32.h>
#include <Input\Win32\SCTKeyboardWin32.h>
#include <Input\Win32\SCTMouseWin32.h>


namespace SCT
{
namespace Input
{

SCTInputManagerWin32::SCTInputManagerWin32(HINSTANCE hInstance)
:	mhInstance (hInstance),
	mpInputDevice (NULL),
	mpKeyboard (NULL),
	mpMouse (NULL)
{
	mbIsKeyboard	= false;
	mbIsMouse		= false;
}

SCTInputManagerWin32::~SCTInputManagerWin32()
{
	Shutdown();
}

SCTReturn SCTInputManagerWin32::Initialize()
{
	HRESULT hr;

	// Initialize the main direct input interface.
	hr = DirectInput8Create(mhInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mpInputDevice, NULL);
	if(hr != DI_OK)
	{
		Core::SCTLogManager::getSingleton().PrintDebug("SCT Input Manager failed to initialized.");
		return FAIL;
	}

	return OK;
}

SCTReturn SCTInputManagerWin32::Update()
{
	if(mbIsKeyboard)
		mpKeyboard->Update();

	if(mbIsMouse)
		mpMouse->Update();

	return OK;
}

SCTReturn SCTInputManagerWin32::Shutdown()
{
	// Shutdown Mouse device
	mbIsMouse	= false;
	delete mpMouse;
	mpMouse	= NULL;

	// Shutdown Keyboard device
	mbIsKeyboard	= false;
	delete mpKeyboard;
	mpKeyboard	= NULL;

	return OK;
}

SCTReturn SCTInputManagerWin32::CreateKeyboardDevice()
{
	mpKeyboard	= new SCTKeyboardWin32(mpInputDevice);
	
	mbIsKeyboard	= true;

	return mpKeyboard->Initialize();
}

SCTReturn SCTInputManagerWin32::CreateMouseDevice()
{
	mpMouse	= new SCTMouseWin32(mpInputDevice);
	
	mbIsMouse	= true;

	return mpMouse->Initialize();
}

}	// End of Input Namespace
}	// End of SCT Namespace