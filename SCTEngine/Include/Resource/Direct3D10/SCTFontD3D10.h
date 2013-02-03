/**
----------------------------------------------------------------------------
	@File:			SCTFontD3D10.h
   
	System:         SCT Game Engine
	Component Name: SCTFontD3D10
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


#ifndef SCT_FONT_D3D10_H
#define SCT_FONT_D3D10_H


#include <Resource\SCTFont.h>


namespace SCT
{
	namespace Resource
	{
		// Foreward declarations
		class SCTTextureD3D10;

		/// Class SCTFontD3D10 
		//  Description: Font Direct3D 10 implementation
		//  ...
		class SCTFontD3D10 : public SCTFont
		{
		public:
			SCTFontD3D10(ID3D10Device *device);
			~SCTFontD3D10();

			SCTReturn			Initialize(char *fontFilename, WCHAR *textureFilename);
			SCTReturn			Update();
			SCTReturn			Shutdown();

			void				BuildVertexArray(char *sentence, void *vertices, float x, float y);

		protected:
			ID3D10Device		*mpDevice;

			SCTFontType			*mpFontType;	// Holds the data that will be used to render the text
			SCTTextureD3D10		*mpTexture;

		private:
			void				LoadFontData(char *fontFilename);

		};
	}	// End of Namespace Resource
}	// End of Namespace SCT

#endif