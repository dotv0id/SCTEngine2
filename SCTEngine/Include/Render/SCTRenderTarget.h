/**
----------------------------------------------------------------------------
	@File:			SCTRenderTarget.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Render Target Interface
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

#ifndef SCT_RENDER_TARGET_H
#define SCT_RENDER_TARGET_H


#include <Render\SCTRenderDefines.h>


namespace SCT
{
	namespace Render
	{
		/// Class SCTRenderTarget
		//  ...
		class SCTRenderTarget
		{
		public:
			virtual				~SCTRenderTarget() {};

			virtual SCTReturn	Initialize(SUInt32 width, SUInt32 height) = 0;
			virtual SCTReturn	Shutdown() = 0;

		protected:

		private:

		};
	}	// End of Render Namespace
}	// End of SCT Namespace

#endif