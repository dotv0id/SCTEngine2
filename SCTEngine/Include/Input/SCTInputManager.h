/**
----------------------------------------------------------------------------
	@File:			SCTInputManager.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Input Manager (Interface)
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


#ifndef SCT_INPUT_MANAGER_H
#define SCT_INPUT_MANAGER_H


#include <SCT.h>
#include <SCTPlatform.h>
#include <SCTSingleton.h>


namespace SCT
{
	namespace Input
	{
		// Foreward declarations
		class SCTKeyboard;
		class SCTMouse;


		/// Class SCTInputManager (Interface)
		//  ...
		class SCTInputManager : public Singleton<SCTInputManager>
		{
		public:
			virtual ~SCTInputManager() {};

			virtual SCTReturn		Initialize() = 0;
			virtual SCTReturn		Update() = 0;
			virtual SCTReturn		Shutdown() = 0;

			virtual SCTReturn		CreateKeyboardDevice() = 0;
			virtual SCTReturn		CreateMouseDevice() = 0;
			
			// Get, Set functions
			virtual SCTKeyboard*	GetKeyboard() = 0;
			virtual SCTMouse*		GetMouse() = 0;

		protected:
			// Bool vars to hold the state of the input interfaces
			bool					mbIsKeyboard;
			bool					mbIsMouse;

		private:

		};	// End of class SCTInputManager
	}	// End of Input Namespace

	// Singlenton
	template<> Input::SCTInputManager* Singleton<Input::SCTInputManager>::ms_Singleton = 0;

}	// End of SCT Namespace

#endif