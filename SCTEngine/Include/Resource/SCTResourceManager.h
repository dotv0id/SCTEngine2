/**
----------------------------------------------------------------------------
	@File:			SCTResourceManager.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Resource Manager
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

#ifndef SCT_RESOURCE_MANAGER_H
#define SCT_RESOURCE_MANAGER_H


#include <SCT.h>
#include <SCTSingleton.h>


namespace SCT
{
	namespace Tools
	{
		class SCTObjLoader;
	}

	namespace Resource
	{
		// Foreward class declarations
		class SCTMeshD3D10;
		class SCTTexture;
		class SCTMaterial;
		class SCTSkyBox;

		// Resource types
		enum SCTResourceType
		{
			eMesh = 0,
			eMaterial,
			eTexture,
			eEnvironmentMap
		};

		// Holds a pointer to a resource
		struct SCTResource
		{
			void				*pResourcePtr;
			SCTResourceType		type;
		};

		/// Class SCTResourceManager
		//  ...
		class SCTResourceManager : public Singleton<SCTResourceManager>
		{
		public:
			SCTResourceManager();
			~SCTResourceManager();

			SCTReturn							Initialize();
			SCTReturn							Shutdown();

			// Get, Set functions
			SCTReturn							GetResource(const char *filename, SCTTexture **rTexture, bool environmentMap = false);
			SCTReturn							GetResource(const char *filename, SCTMaterial **rMaterial);
			SCTReturn							GetResource(const char *filename, SCTMeshD3D10 **rMesh, bool load = true);
			SCTReturn							GetResource(const char *filename, SCTSkyBox **rSkyBox, float radius);

		private:
			std::map<std::string, SCTResource>	mResourceMap;

			// Private Functions
			void								AddResource(const char *filename, SCTResource &resource);
			void								ReleaseResource(SCTResource &resource);
			void								ReleaseAllResources();

			bool								CheckIfLoaded(const char *filename);

			SCTReturn							Load3DModelFromFile(const char *filename, SCTMeshD3D10 *rMesh);
			
		};	// End of class SCTResourceManager
	}	// End of Resource Namespace

	// Singlenton
	template<> Resource::SCTResourceManager* Singleton<Resource::SCTResourceManager>::ms_Singleton = 0;

}	// End of SCT Namespace

#endif