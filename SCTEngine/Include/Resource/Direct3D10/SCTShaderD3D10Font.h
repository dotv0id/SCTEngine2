/**
----------------------------------------------------------------------------
	@File:			SCTShaderD3D10Font.h
   
	System:         SCT Game Engine
	Component Name: SCTShader Font
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

#ifndef SCT_SHADER_D3D10_FONT_H
#define SCT_SHADER_D3D10_FONT_H


#include <Resource\Direct3D10\SCTShaderD3D10.h>


namespace SCT
{
	namespace Resource
	{
		/// Class SCTShaderD3D10Font
		//  used to render wireframe
		class SCTShaderD3D10Font : public SCTShaderD3D10
		{
		public:
			SCTShaderD3D10Font(ID3D10Device *device);
			~SCTShaderD3D10Font();

			SCTReturn					Initialize(WCHAR* filename, char *techniqueName, Render::SCTVertexType type);
			SCTReturn					Render();
			SCTReturn					Shutdown();

			// Set functions
			inline void					SetWVPMatrix(const Math::Matrix4 &WVPMatrix)
			{
				mpWVPMatrix->SetMatrix((float*)&WVPMatrix);
			}

			inline void					SetTexture(ID3D10ShaderResourceView* texture)
			{
				mpTexturePtr->SetResource(texture);
			}

			inline void					SetPixelColor(Math::ColorRGBA *pixelColor)
			{
				mpPixelColorPtr->SetFloatVector((float*)&pixelColor);
			}

		private:
			// Shader Variables
			ID3D10EffectMatrixVariable			*mpWVPMatrix;

			// Textures
			ID3D10EffectShaderResourceVariable	*mpTexturePtr;
			ID3D10EffectVectorVariable			*mpPixelColorPtr;

		};
	}	// End of Namespace Resource
}	// End of Namespace SCT

#endif