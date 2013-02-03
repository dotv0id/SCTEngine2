// -------------------- +
//		Includes
// -------------------- +

#include <Core\SCTWindows.h>
#include <Core\SCTTime.h>


namespace SCT
{
namespace Core
{

// Render window callback
LRESULT CALLBACK RenderWindowCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_ACTIVATE:
		{
			if(LOWORD(wParam) == WA_INACTIVE)			// The app main window is inactive. pause the game
			{
				SCT::Core::SCTTime::getSingletonPtr()->Stop();
				ClipCursor(0);
			}
			else
			{
				SCT::Core::SCTTime::getSingletonPtr()->Start();
				
				// TEST MOUSE CLIPPING 
				// Get window rect.
				RECT WindowRect;
				GetWindowRect(hwnd, &WindowRect);
				
				// Restrict the cursor
				ClipCursor(&WindowRect);
				//\ EOF
			}
		} break;
		
		// Destroy the render window and post a WM_QUIT message
        case WM_DESTROY:
		{
			PostQuitMessage(0);
		} break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}	// End of render window callback

// Config dialog callback
/* --------------------------------------------------------------
 * Function: DialogProc(...)
 * --------------------------------------------------------------
 * Handles the config dialog callback function. 
 *
 * 
 */
INT_PTR CALLBACK ConfigurationDialogCallback(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		// Populate the Combo boxes once the dialog is initialized
		case WM_INITDIALOG:
		{
			/*
			if(gConfigMngr)
			{
				gWin32Mngr->setConfigDialog(hdlg);
				gConfigMngr->populateConfigDialog(hdlg);
			}
			*/

			return true;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDCANCEL:
				{
					//if(gConfigMngr)
					//	gConfigMngr->setIsConfigured(false);	// User chose to "Exit" the app

					SendMessage(hdlg, WM_CLOSE, 0, 0);			// Close the config dialog
					return true;
				}
				
				case IDOK:
				{
					/*
					if(gConfigMngr)
					{
						SCTConfigStruct sctConfig;

						// get configuration from dialog, tell engine ready to go
						if(gConfigMngr->getConfigFromDialog(hdlg, sctConfig))
						{
							gConfigMngr->setEngineConfiguration(sctConfig);
							gConfigMngr->setIsConfigured(true);
						}
					}
					*/
					
					SendMessage(hdlg, WM_CLOSE, 0, 0);	// Close the config dialog
					return true;
				}
			}
			return false;
		}

		// handle close message for configuration dialog
		case WM_CLOSE:
		{
			/*
			if(gConfigMngr)
				gConfigMngr->setIsConfigured(false);	// User chose to "Exit" the app
			*/

			DestroyWindow(hdlg);
			return true;
		}

		// handle destruction of configuration dialog
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return true;
		}
  }
  return false;
}	// End of config dialog callback

// ----------------------------------------- +

	
SCTWindows::SCTWindows() :	mAppInstance (NULL), mMainWindow (NULL), mConfigurationDialog (NULL), 
							mEnableResize (false), mEnableMinimize(false)
{
}

SCTWindows::~SCTWindows()
{
}

SCTReturn SCTWindows::CreateRenderWindow(HINSTANCE AppInstance, const std::wstring &WindowTitle, const SCTConfigRender &Configuration, bool EnableResize, bool EnableMinimize)
{
	mAppInstance			= AppInstance;
	mRenderWindowTitle		= WindowTitle;
	mEnableResize			= EnableResize;
	mEnableMinimize			= EnableMinimize;
	mRenderWindowConfig		= Configuration;
	
	WNDCLASSEX	lWc;
	DEVMODE		lScreenSettings;
	SInt32		lPosX, lPosY;

	// Configure the WNDCLASSEX
	lWc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
	lWc.lpfnWndProc		= RenderWindowCallback;
	lWc.cbClsExtra		= 0;
	lWc.cbWndExtra		= 0;
	lWc.hInstance		= mAppInstance;
	lWc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	lWc.hIconSm			= lWc.hIcon;
	lWc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	lWc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	lWc.lpszMenuName	= NULL;
	lWc.lpszClassName	= mRenderWindowTitle.c_str();
	lWc.cbSize			= sizeof(WNDCLASSEX);

	if(!RegisterClassEx(&lWc))
		return REGISTER_CLASS_FAILED;

	// Screen settings if fullscreen = true
	if(mRenderWindowConfig.Fullscreen)
	{
		memset(&lScreenSettings, 0, sizeof(lScreenSettings));

		lScreenSettings.dmSize       = sizeof(lScreenSettings);
		lScreenSettings.dmPelsWidth  = (unsigned long)mRenderWindowConfig.Resolution.width;
		lScreenSettings.dmPelsHeight = (unsigned long)mRenderWindowConfig.Resolution.height;
		lScreenSettings.dmBitsPerPel = 32;
		lScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&lScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		lPosX = lPosY = 0;
	}else
	{
		// if windowed, used the pre-configured resolution
		// ...

		// If you could place the window in the middle of the screen, it would be great :)
		lPosX = (GetSystemMetrics(SM_CXSCREEN) - mRenderWindowConfig.Resolution.width)  / 2;
		lPosY = (GetSystemMetrics(SM_CYSCREEN) - mRenderWindowConfig.Resolution.height) / 2;
	}

	DWORD lRenderWindowStyle = WS_CAPTION | WS_SYSMENU;

	// Configure the render window
	// Should we allow the window to be minimized?
	if(mEnableMinimize)
		lRenderWindowStyle += WS_MINIMIZEBOX;	

	// Should we allow the window to be resized (& maximized)?
	if(mEnableResize)
		lRenderWindowStyle += WS_MAXIMIZEBOX | WS_THICKFRAME;


	// Create the window with the screen settings and get the handle to it.
	mMainWindow = CreateWindowEx(	WS_EX_APPWINDOW,
									mRenderWindowTitle.c_str(), mRenderWindowTitle.c_str(), 
									lRenderWindowStyle,
									lPosX, lPosY, 
									mRenderWindowConfig.Resolution.width, mRenderWindowConfig.Resolution.height,
									NULL, NULL, mAppInstance, NULL);

	// FAILED TO CREATE WINDOW
	if(!mMainWindow)
		return RENDER_WINDOW_CREATION_FAILED;


	// Show the render window
	ShowWindow(mMainWindow, SW_SHOW);
	UpdateWindow(mMainWindow);
	SetForegroundWindow(mMainWindow);
	SetFocus(mMainWindow);

	return OK;
}
			
SCTReturn SCTWindows::DestroyRenderWindow()
{
	return OK;
}
		
void SCTWindows::SetRenderWindowTitle(const std::wstring &WindowTitle)
{
}
			
HWND SCTWindows::GetRenderWindow()
{
	return mMainWindow;
}

void SCTWindows::SetMouseCursor(bool mode)
{
	ShowCursor(mode);
}

void SCTWindows::CenterMouseCursor()
{
	RECT R;
	GetWindowRect(Core::SCTWindows::getSingleton().GetRenderWindow(), &R);
	SetCursorPos(R.left + mRenderWindowConfig.Resolution.width/2, R.top + mRenderWindowConfig.Resolution.height/2);
}

// --------------------------
// call after you configure
SCTReturn SCTWindows::Initialize()
{
	return OK;
}

SCTReturn SCTWindows::Shutdown()
{
	return OK;
}

/*
void SCTWindows::configureRenderWindow(const SCTConfigStruct &config)
{
	mFullsceen		= config.Fullscreen;
	mMainWndWidth	= config.Resolution.width;
	mMainWndHeight	= config.Resolution.height;
}

void SCTWindows::setMainWndTitle(std::wstring wndTitle)
{
	mMainWndTitle	= wndTitle;
}

void SCTWindows::setRenderWindowStyle(bool allowResize, bool allowMinimize)
{
	mEnableResize	= allowResize;
	mEnableMinimize	= allowMinimize;
}

SCT_RETURN_CODE SCTWindows::Shutdown()
{
	showMouseCursor(true);

	if(mFullsceen)
		ChangeDisplaySettings(NULL, 0);

	// Remove the render window (if it exists)
	if(mMainWnd)
	{
		DestroyWindow(mMainWnd);
		mMainWnd	= NULL;
	}

	// Remove the configuration dialog (if it exists)
	if(mConfigDlg)
	{
		DestroyWindow(mConfigDlg);
		mConfigDlg	= NULL;
	}

	// Remove the application instance
	if(mAppInstance)
	{
		UnregisterClass(mMainWndTitle.c_str(), mAppInstance);
		mAppInstance	= NULL;
	}

	return OK;
}

SCT_RETURN_CODE SCTWindows::showConfigDialog()
{
	InitCommonControls();
	mConfigDlg = CreateDialogParam(mAppInstance, MAKEINTRESOURCE(IDD_CONFIGDLG), 0, cfgDialogCallback, 0);

	if(!mConfigDlg)
		return CONFIG_DIALOG_CREATION_FAILED;

	ShowWindow(mConfigDlg, 1);
	
	return OK;
}

// populate the input combobox with the string elements the cbElementsIter points to
void SCTWindows::populateComboBox(SInt32 comboID, const std::vector<std::wstring> &cbElements)
{
	//mConfigDlg = pfff;
	if(mConfigDlg)
	{
		HWND comboBox = GetDlgItem(mConfigDlg, comboID);

		std::vector<std::wstring>::const_iterator lElementIter;
	
		// Parse the string vector
		for(lElementIter = cbElements.begin();  lElementIter != cbElements.end(); lElementIter++)
			SendMessage(comboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>((LPCTSTR)(*lElementIter).c_str()));
	}
}

void SCTWindows::populateComboBox(SInt32 comboID, const std::wstring cbElement)
{
	if(mConfigDlg)
	{
		HWND comboBox = GetDlgItem(mConfigDlg, comboID);
		SendMessage(comboBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>((LPCTSTR)cbElement.c_str()));
	}
}

HWND SCTWindows::getMainWindow()
{
	return mMainWnd;
}

HWND SCTWindows::getConfigDialog()
{
	return mConfigDlg;
}

void SCTWindows::getComboboxValue(SInt32 cbID, std::wstring &cbValue)
{	
	HWND tmpComboHndl = GetDlgItem(this->mConfigDlg, cbID);

	WCHAR lTmpBuffer[256];
	memset(&lTmpBuffer, 0, sizeof(lTmpBuffer));

	SInt32 nIndex = SendMessage(tmpComboHndl , CB_GETCURSEL, 0, 0 );

	SendMessage(tmpComboHndl, CB_GETLBTEXT , nIndex, (LPARAM)&lTmpBuffer);
	std::wstring lTmpWstring(lTmpBuffer);

	cbValue = lTmpWstring;
}

void SCTWindows::getComboboxValue(SInt32 cbID, std::string &cbValue)
{	
	std::wstring		lTmpWString;
	getComboboxValue(cbID, lTmpWString);

	// Convert wstring to ascii string
	cbValue.assign(lTmpWString.begin(), lTmpWString.end());
}

void SCTWindows::setComboDefaultSelection(SInt32 comboID)
{
	if(mConfigDlg)
	{
		HWND comboBox = GetDlgItem(mConfigDlg, comboID);
		SendMessage(comboBox, CB_SETCURSEL, 0, 0);
	}
}

void SCTWindows::setConfigDialog(HWND hdlg)
{
	mConfigDlg	= hdlg;
}

void SCTWindows::setAppInstance(HINSTANCE hInstance)
{
	mAppInstance = hInstance;
}

bool SCTWindows::isMouseCursorVisible()
{
	return mShowMouseCursor;
}

void SCTWindows::restoreMouseClipArea()
{
	ClipCursor(&mOldCursorClipArea);		// allow mouse cursor to move freely around the whole screen
}

void SCTWindows::confineMouse()
{
	SInt32 lPosX = (GetSystemMetrics(SM_CXSCREEN) - mMainWndWidth)  / 2;
	SInt32 lPosY = (GetSystemMetrics(SM_CYSCREEN) - mMainWndHeight) / 2;

	mCursorClipArea.left		= lPosX;
	mCursorClipArea.right		= lPosX + mMainWndWidth;
	mCursorClipArea.top			= lPosY;
	mCursorClipArea.bottom		= lPosY + mMainWndHeight;

	ClipCursor(&mCursorClipArea);			// confine mouse cursor inside our window only
}
*/
}	// end of Core	namespace
}	// End of SCT	namespace