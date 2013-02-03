/**
----------------------------------------------------------------------------
	@File:			SCTRenderWindowD3D10.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Render Window D3D10 Implementation
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


#ifndef SCT_RENDER_WINDOW_D3D10_H
#define SCT_RENDER_WINDOW_D3D10_H


#include <Render\SCTRenderWindow.h>


namespace SCT
{
	namespace Render
	{
		class SCTRenderInterfaceD3D10;

		/// Class SCTRenderWindow
		//  ...
		class SCTRenderWindowD3D10 : public SCTRenderWindow
		{
		public:
			SCTRenderWindowD3D10(ID3D10Device *device);
			~SCTRenderWindowD3D10();

			SCTReturn					Initialize(	SInt32 screenWidth, SInt32 screenHeight,
													SInt32 windowWidth, SInt32 windowHeight);
			SCTReturn					Render(SInt32 positionX, SInt32 positionY, SInt32 windowWidth, SInt32 windowHeight);
			SCTReturn					Shutdown();

		protected:

		private:
			// --- Private Variables ---
			ID3D10Device				*mpDevice;

			// 2D Sprite texture
			//Resource::SCTTextureD3D10	*mpDXTexture;

			ID3D10Buffer				*mpDXVertexBuffer;
			ID3D10Buffer				*mpDXIndexBuffer;

			SUInt32						mVertexCount,
										mIndexCount;

			SInt32						mPositionX, 
										mPositionY,
										mScreenWidth,	// Screen
										mScreenHeight,	// resolution
										mWidth,			// Render window 
										mHeight;		// resolution

			// --- Private Functions ---
			SCTReturn					UpdateBuffers(SInt32 positionX, SInt32 positionY, SInt32 windowWidth, SInt32 windowHeight);

		public:
			friend class SCTRenderInterfaceD3D10;
		};
	}	// End of Render Namespace
}	// End of SCT Namespace

#endif