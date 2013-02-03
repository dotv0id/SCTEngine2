/**
----------------------------------------------------------------------------
	@File:			SCTShader.h
   
	System:         SCT Game Engine
	Component Name: SCTShader
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


#ifndef SCT_SHADER_H
#define SCT_SHADER_H


#include <SCT.h>
#include <Render\SCTRenderDefines.h>
#include <Render\SCTVertexDefs.h>


namespace SCT
{
	namespace Resource
	{
		/// Class SCTShader 
		//  Description: Shader interface
		//  ...
		class SCTShader
		{
		public:
			virtual				~SCTShader() {}

			virtual SCTReturn	Initialize(WCHAR* filename, char *techniqueName, Render::SCTVertexType type) = 0;
			virtual	SCTReturn	Render() = 0;
			virtual SCTReturn	Shutdown() = 0;

		protected:			
		};
	}	// End of Namespace Resource
}	// End of Namespace SCT

#endif