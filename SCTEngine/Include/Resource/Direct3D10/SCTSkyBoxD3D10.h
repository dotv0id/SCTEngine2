/**
----------------------------------------------------------------------------
	@File:			SCTSkyBoxD3D10.h
   
	System:         SCT Game Engine 
	Component Name:	SCT Scene: Skybox
	Status:         Version 1.0 Release 1 

	Language: C++
   
	License: GNU Public License	[*!]
	Licensed Material - Property of ...
   
	Author: Anastasios 'v0id' Giannakopoulos
	E-Mail: dotvoidd@gmail.com
   
	Description:	Skybox 
 
	Limitations:
   
	Function:

----------------------------------------------------------------------------
*/


#ifndef _SCT_SKY_BOX_D3D10_H_
#define _SCT_SKY_BOX_D3D10_H_


#include <Resource\SCTSkyBox.h>
#include <Render\SCTRenderDefines.h>


namespace SCT
{
	namespace Resource
	{
		/// Class SCTSkyBox
		//  ...
		class SCTSkyBoxD3D10 : public SCTSkyBox
		{
		public:
			SCTSkyBoxD3D10(ID3D10Device *device);
			~SCTSkyBoxD3D10();

			SCTReturn				Initialize(Resource::SCTTexture *cubeMapTexture, float radius);
			SCTReturn				Render();
			SCTReturn				Shutdown();

			SUInt32					GetIndexCount();

		protected:

		private:
			ID3D10Device			*mpDevice;
		};

	}	// End of Scene Namespace
}	// End of SCT Namespace


#endif