/**
----------------------------------------------------------------------------
	@File:			SCTMesh.h
   
	System:         SCT Game Engine
	Component Name: SCTMesh
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


#ifndef SCT_INPUT_LAYOUT_H
#define SCT_INPUT_LAYOUT_H


#include <Render\SCTRenderDefines.h>
#include <Render\SCTVertexDefs.h>


namespace SCT
{
	namespace Render
	{
		/// Input Layout Interface
		//  ...
		class SCTInputLayout
		{
		public:
			virtual				~SCTInputLayout() {}
			virtual SCTReturn	Shutdown() = 0;

		protected:

		};
	}	// End of Namespace Render
}	// End of Namespace SCT

#endif