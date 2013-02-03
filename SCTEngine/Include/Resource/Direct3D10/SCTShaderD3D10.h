/**
----------------------------------------------------------------------------
	@File:			SCTShaderD3D10.h
   
	System:         SCT Game Engine
	Component Name: SCTShaderD3D10
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


#ifndef SCT_SHADER_D3D10_H
#define SCT_SHADER_D3D10_H


#include <Resource\SCTShader.h>


namespace SCT
{
	// Foreward declaration of classes
	namespace Render
	{
		class	SCTInputLayoutD3D10;
		class	SCTRenderInterfaceD3D10;
	}

	namespace Resource
	{
		/// Class SCTShaderD3D10
		//  Description: Shader Direct3D 10 implementation
		//  ...
		class SCTShaderD3D10 : public SCTShader
		{
		public:
			SCTShaderD3D10() {};
			SCTShaderD3D10(ID3D10Device *device);
			~SCTShaderD3D10();

			SCTReturn					Initialize(WCHAR* filename, char *techniqueName, Render::SCTVertexType type);
			SCTReturn					Render();
			SCTReturn					Shutdown();

		protected:
			ID3D10Device				*mpDevice;

			ID3D10Effect				*mpShaderEffect;
			ID3D10EffectTechnique		*mpTechnique;
			Render::SCTInputLayoutD3D10 *mpInputLayout;

		public:
			// Friend Classes
			friend class				Render::SCTInputLayoutD3D10;
			friend class				Render::SCTRenderInterfaceD3D10;

		};

	}	// End of Namespace Resource
}	// End of Namespace SCT

#endif