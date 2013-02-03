/**
----------------------------------------------------------------------------
	@File:			SCTTexture.h
   
	System:         SCT Game Engine
	Component Name: SCTTexture
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


#ifndef SCT_TEXTURE_H
#define SCT_TEXTURE_H


#include <Render\SCTRenderDefines.h>


namespace SCT
{
	namespace Resource
	{
		/// Class SCTTexture 
		//  Description: Texture interface
		//  ...
		class SCTTexture
		{
		public:
			virtual				~SCTTexture() {};

			virtual SCTReturn	Initialize(const WCHAR *filename, bool asCubeMap = false)	= 0;
			virtual SCTReturn	Initialize(SUInt32 width, SUInt32 height, SUInt32 flags)	= 0;

			virtual SCTReturn	Shutdown()		= 0;

		protected:

		};
	}	// End of Namespace Resource
}	// End of Namespace SCT

#endif