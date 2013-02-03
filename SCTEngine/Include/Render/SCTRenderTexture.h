/**
----------------------------------------------------------------------------
	@File:			SCTRenderTexture.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Render Texture Interface
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

#ifndef SCT_RENDER_TEXTURE_H
#define SCT_RENDER_TEXTURE_H


#include <Render\SCTRenderTarget.h>


namespace SCT
{
	namespace Render
	{
		/// Class SCTRenderTexture
		//  ...
		class SCTRenderTexture : public SCTRenderTarget
		{
		public:
			virtual ~SCTRenderTexture() {};

			virtual SCTReturn		Initialize(SUInt32 width, SUInt32 height) = 0;
			virtual SCTReturn		Shutdown() = 0;
		
		protected:
			//Resource::SCTTexture	*mpRenderTargetTexture;

		};
	}	// End of Render Namespace
}	// End of SCT Namespace

#endif