/**
----------------------------------------------------------------------------
	@File:			SCTWindows.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Win32 abstraction
	Status:         Version 1.0 Release 1 

	Language: C++
   
	License: GNU Public License	[*!]
	Licensed Material - Property of ...
   
	Author: Anastasios 'v0id' Giannakopoulos
	E-Mail: dotvoidd@gmail.com
   
	Description:	Header file for Project X
					This file contains the defined types for Project X
					This is sometimes called the "Abstract" and may be
					followed by a section called "Notes".
 
	Limitations:
   
	Function:

----------------------------------------------------------------------------
*/


#ifndef SCT_WINDOWS_H
#define SCT_WINDOWS_H


#include <SCT.h>
#include <SCTSingleton.h>
#include <SCTPlatform.h>


namespace SCT
{
	namespace Core
	{
		// Class name:	SCTWin32
		// Description: provides string manipulation functions
		// ...
		class SCTWindows : public Singleton<SCTWindows>
		{
		public:
			SCTWindows();
			~SCTWindows();

			SCTReturn			Initialize();
			SCTReturn			Shutdown();

			SCTReturn			CreateRenderWindow(	HINSTANCE AppInstance,
													const std::wstring &WindowTitle, 
													const SCTConfigRender &Configuration,
													bool EnableResize, bool EnableMinimize);
			
			SCTReturn			DestroyRenderWindow();

			// Get, Set Functions
			void				SetRenderWindowTitle(const std::wstring &WindowTitle);

			HWND				GetRenderWindow();

			inline void			GetWindowSize(SInt32 &width, SInt32 &height)
			{
				RECT r;
				GetWindowRect(mMainWindow, &r);

				width	= abs(r.right - r.left);
				height	= abs(r.bottom - r.top);
			}

			void				SetMouseCursor(bool mode);

			void				CenterMouseCursor();
			// -------
			/*
			SCT_RETURN_CODE		showConfigDialog();

			void				populateComboBox(SInt32 comboID, const std::vector<std::wstring> &cbElements);		// list of elements
			void				populateComboBox(SInt32 comboID, const std::wstring cbElement);					// single element

			void				setMainWndTitle(std::wstring wndTitle);
			void				configureRenderWindow(const SCTConfigStruct &config);
			void				setRenderWindowStyle(bool allowResize, bool allowMinimize);
	
			HWND				getMainWindow();
			HWND				getConfigDialog();
			void				setConfigDialog(HWND hdlg);

			void				showMouseCursor(bool _show);
			bool				isMouseCursorVisible();
			void				restoreMouseClipArea();
			void				confineMouse();

			// Gets the current value of a combobox in the config dialog 
			void				getComboboxValue(SInt32 cbID, std::wstring &cbValue);
			void				getComboboxValue(SInt32 cbID, std::string &cbValue);

			void				setComboDefaultSelection(SInt32 comboID);
	
			void				setAppInstance(HINSTANCE hInstance);
			*/

		private:
			HINSTANCE			mAppInstance;						// Application instance
			HWND				mMainWindow;						// Render window hanlder
			HWND				mConfigurationDialog;				// Configuration dialog handler

			std::wstring		mRenderWindowTitle;

			SCTConfigRender		mRenderWindowConfig;

			bool				mEnableResize, 
								mEnableMinimize;


			// -----------------------
			//RECT				mCursorClipArea;				// new area for ClipCursor
			//RECT				mOldCursorClipArea;

		};	// End of SCTWin32 Class definition
	}	// End of Core Namespace

	// Singlenton
	template<> Core::SCTWindows* Singleton<Core::SCTWindows>::ms_Singleton = 0;

}	// End of SCT Namespace

#endif