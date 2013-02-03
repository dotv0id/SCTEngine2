/**
----------------------------------------------------------------------------
	@File:			SCTFont.h
   
	System:         SCT Game Engine
	Component Name: SCTFont
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

#ifndef SCT_FONT_H
#define SCT_FONT_H


#include <Render\SCTRenderDefines.h>


namespace SCT
{
	namespace Resource
	{
		/// Class SCTFont 
		//  Description: Font interface
		//  ...
		class SCTFont
		{
		public:
			virtual				~SCTFont() {}

			virtual SCTReturn	Initialize(char *fontFilename, WCHAR *textureFilename)	= 0;
			virtual SCTReturn	Update()		= 0;
			virtual SCTReturn	Shutdown()		= 0;

		protected:

		};		
	}	// End of Namespace Resource
}	// End of Namespace SCT

#endif