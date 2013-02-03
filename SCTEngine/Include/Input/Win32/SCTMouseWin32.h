/**
----------------------------------------------------------------------------
	@File:			SCTMouseWin32.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Mouse Win32 Implementation
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

#ifndef SCT_MOUSE_WIN32_H
#define SCT_MOUSE_WIN32_H


#include <Input\SCTMouse.h>
#include <dinput.h>


namespace SCT
{
	namespace Input
	{
		/// Interface Mouse
		//  ...
		class SCTMouseWin32 : public SCTMouse
		{
		public:
			SCTMouseWin32(IDirectInput8 *inputDevice);
			~SCTMouseWin32();

			SCTReturn				Initialize();
			SCTReturn				Update();
			SCTReturn				Shutdown();

			bool					IsButtonUp(SCTMouseCode buttonID);
			bool					IsButtonDown(SCTMouseCode buttonID);

			// Get, Set functions
			bool					GetMouseState(SCTMouseCode buttonID);

			inline void				GetAbsolutePosition(SInt32 &x, SInt32 &y)
			{
				x	= mMouseX;
				y	= mMouseY;
			}

			inline SInt32			GetRelativeX()
			{
				return (SInt32)mMouseState.lX;
			}

			inline SInt32			GetRelativeY()
			{
				return (SInt32)mMouseState.lY;
			}

			inline SInt32			GetRelativeZ()
			{
				return (SInt32)mMouseState.lZ;
			}

		private:
			// --- Private Variables ---
			IDirectInput8			*mpInputDevice;

			IDirectInputDevice8		*mpMouse;
			DIMOUSESTATE			mMouseState;
			
			// Mouse button states
			bool					mButtonState[SCT_NUM_MOUSECODES];

			SInt32					mScreenWidth, 
									mScreenHeight,
									mMouseX, 
									mMouseY;

			// --- Private functions ---
			SCTReturn				UpdateMouseDevice();

		};

	}	// End of Input Namespace
}	// End of SCT Namespace

#endif