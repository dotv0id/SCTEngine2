/**
----------------------------------------------------------------------------
	@File:			SCTShaderD3D10Wireframe.h
   
	System:         SCT Game Engine
	Component Name: SCTShader Wireframe
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

#ifndef SCT_SHADER_D3D10_WIREFRAME_H
#define SCT_SHADER_D3D10_WIREFRAME_H


#include <Resource\Direct3D10\SCTShaderD3D10.h>


namespace SCT
{
	namespace Resource
	{
		/// Class SCTShaderD3D10Wireframe
		//  used to render wireframe
		class SCTShaderD3D10Wireframe : public SCTShaderD3D10
		{
		public:
			SCTShaderD3D10Wireframe(ID3D10Device *device);
			~SCTShaderD3D10Wireframe();

			SCTReturn					Initialize(WCHAR* filename, char *techniqueName, Render::SCTVertexType type);
			SCTReturn					Render();
			SCTReturn					Shutdown();

			inline void					SetWVPMatrix(const Math::Matrix4 &WVPMatrix)
			{
				mpWVPMatrix->SetMatrix((float*)&WVPMatrix);
			}

			inline void					SetWireframeColor(const Math::ColorRGBA &color)
			{
				mpWireframeColor->SetFloatVector((float*)&color);
			}

		private:
			// Shader Variables
			ID3D10EffectMatrixVariable	*mpWVPMatrix;
			ID3D10EffectVectorVariable	*mpWireframeColor;

		};
	}	// End of Namespace Resource
}	// End of Namespace SCT

#endif