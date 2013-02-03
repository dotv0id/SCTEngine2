/**
----------------------------------------------------------------------------
	@File:			SCTVegetation.h
   
	System:         SCT Game Engine 
	Component Name:	SCT Vegetation
	Status:         Version 1.0 Release 1 

	Language: C++
   
	License: GNU Public License	[*!]
	Licensed Material - Property of ...
   
	Author: Anastasios 'v0id' Giannakopoulos
	E-Mail: dotvoidd@gmail.com
   
	Description:	Procedurally generation of vegetation,
					[!] more like 'procedural positioning of 
					vegetation entities', at the momment
 
	Limitations:
   
	Function:

----------------------------------------------------------------------------
*/

#ifndef _SCT_VEGETATION_H_
#define _SCT_VEGETATION_H_


#include <SCT.h>
#include <SCTSingleton.h>
#include <SCTMath.h>
#include <PCG\SCTPCGTypeDefs.h>


namespace SCT
{
	// Foreward declarations
	namespace Resource
	{
		class SCTMesh;
	}

	namespace Scene
	{
		class SCTSceneNode;
	}

	namespace PCG
	{
		/// Class SCTVegetationEntity
		//  ...
		class SCTVegetationEntity
		{
		public:
			SCTVegetationEntity() {};
			~SCTVegetationEntity() {};

			// Enum of the different vegetation entity types
			enum SCTVegetationType
			{
				eTree = 0,
				eFoliage,
				eMisc,
				eMaxTypes
			};

		private:
			// Unique id for this entity
			SUInt32								mID;

			// The name of this entity
			std::string							mName;

			// The 3D model this entity will use
			Resource::SCTMesh					*mpMesh;

			// Min range is used to place items not related 
			// to this entity closer, while the max radius to 
			// place related entities away.
			Math::Range							mRadiusRange;
			float								mYOffset;

			SCTVegetationType					mType;

		public:
			friend class SCTVegetation;
		};


		/// Class SCTVegetation
		//  ...
		class SCTVegetation : public Singleton<SCTVegetation>
		{
		public:
			SCTVegetation();
			~SCTVegetation();

			SCTReturn			Initialize();
			SCTReturn			Shutdown();

			SCTReturn			GenerateVegetation(const SCTVegetationConfig &config, Scene::SCTSceneNode *terrain);
			SCTReturn			CreateVegetationEntity(	const std::string &name, Resource::SCTMesh *mesh, 
														float minRange, float maxRange, 
														float yOffset,
														SCTVegetationEntity::SCTVegetationType type);
			SCTReturn			AddRelationship(const std::string &entity_1, const std::string &entity_2);

		protected:

		private:
			// Map of entity relationships
			// Description: tells the vegetation manager
			//				which entity affects which
			struct SCTEntityRelationship
			{
				SUInt32 first,
						second;
			};
			std::vector<SCTEntityRelationship>		mRelationships;

			// List of all the available Vegetation Entities
			// the manager can use to create the forest
			std::vector<SCTVegetationEntity>		mEntities;

			// --- Private Functions ---
			// Returns true if this ID already exists inside the mEntities map
			bool									CheckDuplicateID(SUInt32 id);
			bool									IsRelated(SUInt32 id_1, SUInt32 id_2);
		};

	}	// End of PCG namespace

	// Singlenton
	template<> PCG::SCTVegetation* Singleton<PCG::SCTVegetation>::ms_Singleton = 0;

}	// End of SCT namespace

#endif