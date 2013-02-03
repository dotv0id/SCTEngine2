/**
----------------------------------------------------------------------------
	@File:			SCTImageLoader.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Core: Image Loader
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


#ifndef SCT_IMAGE_LOADER_H
#define SCT_IMAGE_LOADER_H


#include <SCT.h>
#include <SCTSingleton.h>


namespace SCT
{
	namespace Core
	{
		/// Class SCTImageLoader
		//  ...
		class SCTImageLoader : public Singleton<SCTImageLoader>
		{
		public:
			SCTImageLoader();
			~SCTImageLoader();

			SCTReturn		Initialize();
			SCTReturn		Shutdown();

			SCTReturn		LoadImageFromFile(	const wchar_t * filename, 
												std::vector<ColorRGB> &rawImageData, 
												bool resize = false, 
												SUInt32 width = 0, 
												SUInt32 height = 0);

		private:

		};
	}	// End of Core Namespace

	// Singlenton
	template<> Core::SCTImageLoader* Singleton<Core::SCTImageLoader>::ms_Singleton = 0;
}	// End of SCT Namespace

#endif