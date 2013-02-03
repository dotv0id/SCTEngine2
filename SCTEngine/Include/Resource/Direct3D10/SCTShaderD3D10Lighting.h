/**
----------------------------------------------------------------------------
	@File:			SCTShaderD3D10Lighting.h
   
	System:         SCT Game Engine
	Component Name: SCTShader Lighting
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

#ifndef SCT_SHADER_D3D10_LIGHTING_H
#define SCT_SHADER_D3D10_LIGHTING_H


#include <Resource\Direct3D10\SCTShaderD3D10.h>
#include <Resource\SCTMaterial.h>


namespace SCT
{
	namespace Resource
	{
		/// Class SCTShaderD3D10Lighting
		//  used to render wireframe
		class SCTShaderD3D10Lighting : public SCTShaderD3D10
		{
		public:
			SCTShaderD3D10Lighting(ID3D10Device *device);
			~SCTShaderD3D10Lighting();

			SCTReturn					Initialize(WCHAR* filename, char *techniqueName, Render::SCTVertexType type);
			SCTReturn					Render();
			SCTReturn					Shutdown();

			inline void					SetWorldMatrix(const Math::Matrix4 &worldMatrix)
			{
				mpWorldMatrix->SetMatrix((float*)&worldMatrix);
			}

			inline void					SetViewMatrix(const Math::Matrix4 &viewMatrix)
			{
				mpViewMatrix->SetMatrix((float*)&viewMatrix);
			}

			inline void					SetProjectionMatrix(const Math::Matrix4 &projectionMatrix)
			{
				mpProjectionMatrix->SetMatrix((float*)&projectionMatrix);
			}

			inline void					SetCameraPosition(const Math::Vector3 &cameraPos)
			{
				mpCameraPosition->SetFloatVector((float*)&cameraPos);
			}

			inline void					SetTextures(ID3D10ShaderResourceView** textures)
			{
				mpTexturePtr->SetResourceArray(textures, 0, MAX_TEXTURES);
			}

			inline void					SetFogParameters(const Math::ColorRGBA &colour, float start, float end)
			{
				mpFogColour->SetFloatVector((float*)&colour);
				mpFogStart->SetFloat(start);
				mpFogEnd->SetFloat(end);
			}

			inline void					SetRenderTextures(bool mode)
			{
				mpRenderTextures->SetBool(mode);
			}

			inline void					SetMaterialProperties(const Resource::SCTMaterial::MaterialProperties &material)
			{
				mpMaterial->SetRawValue((void*)&material, 0, sizeof(Resource::SCTMaterial::MaterialProperties));
			}

		private:
			// Shader Variables
			ID3D10EffectMatrixVariable			*mpWorldMatrix;
			ID3D10EffectMatrixVariable			*mpViewMatrix;
			ID3D10EffectMatrixVariable			*mpProjectionMatrix;

			ID3D10EffectVectorVariable			*mpCameraPosition;
			ID3D10EffectVectorVariable			*mpFogColour;

			ID3D10EffectScalarVariable			*mpFogStart;
			ID3D10EffectScalarVariable			*mpFogEnd;
			ID3D10EffectScalarVariable			*mpRenderTextures;

			ID3D10EffectVariable				*mpMaterial;

			// Textures
			ID3D10EffectShaderResourceVariable	*mpTexturePtr;

		};
	}	// End of Resource Namespace
}	// End of SCT Namespace

#endif