/**
----------------------------------------------------------------------------
	@File:			SCTRenderWindow.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Render Window Interface
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


#ifndef SCT_RENDER_WINDOW_H
#define SCT_RENDER_WINDOW_H


#include <Render\SCTRenderDefines.h>


namespace SCT
{
	namespace Render
	{
		/// Class SCTRenderWindow
		//  Decription: Represents a target render window that will be rendered
		//				on the screen
		class SCTRenderWindow // should inherit from 2DSprite
		{
		public:
			virtual					~SCTRenderWindow() {};

			virtual SCTReturn		Initialize(	SInt32 screenWidth, SInt32 screenHeight,
												SInt32 windowWidth, SInt32 windowHeight) = 0;
			virtual SCTReturn		Render(SInt32 positionX, SInt32 positionY, SInt32 windowWidth, SInt32 windowHeight) = 0;
			virtual SCTReturn		Shutdown() = 0;

		protected:

		private:

		};
	}	// End of Render Namespace
}	// End of SCT Namespace

#endif