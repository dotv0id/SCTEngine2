/**
----------------------------------------------------------------------------
	@File:			SCTSkyBox.h
   
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


#ifndef _SCT_SKY_BOX_H_
#define _SCT_SKY_BOX_H_


#include <SCT.h>


namespace SCT
{
	// Foreward declarations
	namespace Resource
	{
		class SCTSubmesh;
		class SCTTexture;
	}

	namespace Resource
	{
		/// Class SCTSkyBox
		//  ...
		class SCTSkyBox
		{
		public:
			virtual ~SCTSkyBox() {};

			virtual SCTReturn				Initialize(Resource::SCTTexture *cubeMapTexture, float radius) = 0;
			virtual SCTReturn				Render() = 0;
			virtual SCTReturn				Shutdown() = 0;

			// Get, Set Functions
			virtual SUInt32					GetIndexCount() = 0;
			
			inline Resource::SCTTexture*	GetTexture()
			{
				return mpCubeMapTexture;
			}

			inline void						SetTexture(Resource::SCTTexture *cubeMapTexture)
			{
				mpCubeMapTexture = cubeMapTexture;
			}

		protected:
			Resource::SCTSubmesh			*mpGeometry;
			Resource::SCTTexture			*mpCubeMapTexture;

		private:
			
		};

	}	// End of Scene Namespace
}	// End of SCT Namespace


#endif