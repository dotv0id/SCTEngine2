/**
----------------------------------------------------------------------------
	@File:			SCTTexture.h
   
	System:         SCT Game Engine
	Component Name: SCTTexture
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

#ifndef SCT_TEXTURE_D3D10_H
#define SCT_TEXTURE_D3D10_H


#include <Resource\SCTTexture.h>


namespace SCT
{
	class SCTEngine;	// temp

	// Foreward declaration of classes
	namespace Render
	{
		class SCTRenderInterfaceD3D10;
		class SCTRenderTextureD3D10;
	}

	namespace Resource
	{
		class SCTMeshD3D10;
		class SCTFontD3D10;
	}

	namespace Resource
	{
		/// Class SCTTextureD3D10 
		//  Description: Texture Direct3D 10 implementation
		//  ...
		class SCTTextureD3D10 : public SCTTexture
		{
		public:
			SCTTextureD3D10(ID3D10Device *device);
			~SCTTextureD3D10();

			SCTReturn					Initialize(const WCHAR *filename, bool asCubeMap = false);	// Loads a texture from file
			SCTReturn					Initialize(SUInt32 width, SUInt32 height, SUInt32 flags);	// Creates a custom texture
			SCTReturn					Initialize(ID3D10Texture2D* depthStencil);
			SCTReturn					Shutdown();

		protected:
			ID3D10Device				*mpDevice;

			ID3D10Texture2D				*mDXTexture;
			ID3D10ShaderResourceView	*mpShaderResourceView;

		public:
			// Friend Classes
			friend class				Resource::SCTMeshD3D10;
			friend class				Resource::SCTFontD3D10;
			friend class				Render::SCTRenderInterfaceD3D10;			
			friend class				Render::SCTRenderTextureD3D10;
			friend class				SCTEngine;
		};
	}	// End of Namespace Resource
}	// End of Namespace SCT

#endif