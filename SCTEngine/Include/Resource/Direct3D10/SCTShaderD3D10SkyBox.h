/**
----------------------------------------------------------------------------
	@File:			SCTShaderD3D10SkyBox.h
   
	System:         SCT Game Engine 
	Component Name:	SCT Scene: Skybox shader
	Status:         Version 1.0 Release 1 

	Language: C++
   
	License: GNU Public License	[*!]
	Licensed Material - Property of ...
   
	Author: Anastasios 'v0id' Giannakopoulos
	E-Mail: dotvoidd@gmail.com
   
	Description:	Skybox shader D3D10
 
	Limitations:
   
	Function:

----------------------------------------------------------------------------
*/


#ifndef _SCT_SHADER_D3D10_SKY_BOX_H_
#define _SCT_SHADER_D3D10_SKY_BOX_H_


#include <Resource\Direct3D10\SCTShaderD3D10.h>


namespace SCT
{
	namespace Resource
	{
		/// Class SCTShaderD3D10SkyBox
		//  used to render the Skybox
		class SCTShaderD3D10SkyBox : public SCTShaderD3D10
		{
		public:
			SCTShaderD3D10SkyBox(ID3D10Device *device);
			~SCTShaderD3D10SkyBox();

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

			inline void					SetTexture(ID3D10ShaderResourceView* texture)
			{
				mpTexturePtr->SetResource(texture);
			}

		private:
			ID3D10EffectMatrixVariable			*mpWorldMatrix;
			ID3D10EffectMatrixVariable			*mpViewMatrix;
			ID3D10EffectMatrixVariable			*mpProjectionMatrix;

			// Environment map Texture
			ID3D10EffectShaderResourceVariable	*mpTexturePtr;
		};

	}	// End of Resource Namespace
}	// End of SCT Namespace


#endif