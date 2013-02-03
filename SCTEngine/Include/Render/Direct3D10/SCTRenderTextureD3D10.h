/**
----------------------------------------------------------------------------
	@File:			SCTRenderTextureD3D10.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Render Texture D3D10 Implementation
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

#ifndef SCT_RENDER_TEXTURE_D3D10_H
#define SCT_RENDER_TEXTURE_D3D10_H


#include <Render\SCTRenderTexture.h>


namespace SCT
{
	// Foreward declaration of classes
	class SCTEngine;

	namespace Resource
	{
		class SCTTextureD3D10;
	}

	namespace Render
	{
		// Foreward declaration of classes
		class SCTRenderInterfaceD3D10;

		/// Class SCTRenderTextureD3D10
		//  ...
		class SCTRenderTextureD3D10 : public SCTRenderTexture
		{
		public:
			SCTRenderTextureD3D10(ID3D10Device *device);
			~SCTRenderTextureD3D10();

			SCTReturn					Initialize(SUInt32 width, SUInt32 height);
			SCTReturn					Shutdown();

		protected:
			ID3D10Device				*mpDevice;
			ID3D10RenderTargetView		*mpRenderTargetView;

			Resource::SCTTextureD3D10	*mpRenderTargetTexture;
		
		public:			
			friend class SCTRenderInterfaceD3D10;
			friend class SCTEngine;
		};
	}	// End of Render Namespace
}	// End of SCT Namespace

#endif