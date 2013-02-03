/**
----------------------------------------------------------------------------
	@File:			SCTShaderD3D10PostEffect.h
   
	System:         SCT Game Engine
	Component Name: SCTShader Post Effect Shader
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

#ifndef SCT_SHADER_D3D10_POST_EFFECT_H
#define SCT_SHADER_D3D10_POST_EFFECT_H


#include <Resource\Direct3D10\SCTShaderD3D10.h>


namespace SCT
{
	namespace Resource
	{
		/// Class SCTShaderD3D10PostEffect
		//  used to render wireframe
		class SCTShaderD3D10PostEffect : public SCTShaderD3D10
		{
		public:
			SCTShaderD3D10PostEffect(ID3D10Device *device);
			~SCTShaderD3D10PostEffect();

			SCTReturn					Initialize(WCHAR* filename, char *techniqueName, Render::SCTVertexType type);
			SCTReturn					Render();
			SCTReturn					Shutdown();

			inline void					SetWVPMatrix(const Math::Matrix4 &WVPMatrix)
			{
				mpWVPMatrix->SetMatrix((float*)&WVPMatrix);
			}

			inline void					SetPostEffectTexture(ID3D10ShaderResourceView* texture)
			{
				mpTexturePtr->SetResource(texture);
			}

			inline void					SetDepthBufferTexture(ID3D10ShaderResourceView* texture)
			{
				mpDepthTexturePtr->SetResource(texture);
			}

			inline void					SetViewportSize(float width, float height)
			{
				mpVpWidth->SetFloat(width);
				mpVpHeight->SetFloat(height);
			}

			inline void					SetDoFParameters(float a, float width, float height, float focalPlaneDepth)
			{
				float DoFParams[4] = { a, width, height, focalPlaneDepth };

				mpDoFParams->SetFloatVector((float*)DoFParams);
			}

			inline void					SetProjectionMatrix(const Math::Matrix4 &projectionMatrix)
			{
				mpProjectionMatrix->SetMatrix((float*)&projectionMatrix);
			}

			inline void					SetDownscaleResolution(float width, float height)
			{
				float params[2] = {width, height};
				mpDownscaleResolution->SetFloatVector((float*)params);
			}

		private:
			// Shader Variables
			ID3D10EffectMatrixVariable			*mpWVPMatrix;
			ID3D10EffectMatrixVariable			*mpProjectionMatrix;
			ID3D10EffectScalarVariable			*mpVpWidth,
												*mpVpHeight;
			
			ID3D10EffectVectorVariable			*mpDoFParams;
			ID3D10EffectVectorVariable			*mpDownscaleResolution;

			// Textures
			ID3D10EffectShaderResourceVariable	*mpTexturePtr;
			ID3D10EffectShaderResourceVariable	*mpDepthTexturePtr;

		};
	}	// End of Namespace Resource
}	// End of Namespace SCT

#endif