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

#ifndef SCT_INPUT_LAYOUT_D3D10_H
#define SCT_INPUT_LAYOUT_D3D10_H


#include <Render\SCTInputLayout.h>


namespace SCT
{
	// Foreward declaration of classes
	namespace Resource
	{
		class SCTShaderD3D10;
		class SCTMeshD3D10;
	}

	namespace Render
	{
		// Foreward declaration of classes
		class SCTRenderInterfaceD3D10;

		/// Input Layout implementation for Direct3D 10
		//  ...
		class SCTInputLayoutD3D10 : public SCTInputLayout
		{
		public:			
			SCTInputLayoutD3D10(ID3D10Device *device, SCTVertexType type, Resource::SCTShaderD3D10 *shader, char *techniqueName);
			~SCTInputLayoutD3D10();

			SCTReturn			Shutdown();
						
		protected:
			ID3D10Device		*mpDevice;
			ID3D10InputLayout	*mpDXInputLayout;
			
			SCTVertexType		mVertexType;
			
		public:
			// Friend Classes
			friend class		Render::SCTRenderInterfaceD3D10;
			friend class		Resource::SCTShaderD3D10;
			friend class		Resource::SCTMeshD3D10;

		};
	}	// End of Namespace Render
}	// End of Namespace SCT

#endif