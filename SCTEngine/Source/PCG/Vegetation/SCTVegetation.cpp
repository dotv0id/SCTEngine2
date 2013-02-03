#include <SCTCore.h>
#include <PCG\Vegetation\SCTVegetation.h>
#include <Resource\Direct3D10\SCTMeshD3D10.h>
#include <Scene\SCTSceneManager.h>
#include <Scene\SCTSceneNode.h>


namespace SCT
{
namespace PCG
{

SCTVegetation::SCTVegetation()
{
}

SCTVegetation::~SCTVegetation()
{
}

SCTReturn SCTVegetation::Initialize()
{
	return OK;
}

SCTReturn SCTVegetation::Shutdown()
{
	return OK;
}

SCTReturn SCTVegetation::GenerateVegetation(const SCTVegetationConfig &config, Scene::SCTSceneNode *terrain)
{
	Scene::SCTSceneManager *sceneMngr	= Scene::SCTSceneManager::getSingletonPtr();

	std::vector<float>	distance;		// Used in raycasting - to get the distance of the collision from the origin

	// Normalize the percentages in config to
	SCTVegetationConfig vegConfig	= config;
	vegConfig.treePercentage	/= 100;
	vegConfig.foliagePercentage	/= 100;
	vegConfig.miscPercentage	 /= 100;

	struct entry
	{
		SUInt32			id;
		Math::Vector3	position;
	};

	// Temp map to hold pozition - entity association
	//std::map<SUInt32, Math::Vector3> entityPositions;
	std::vector<entry> entityPositions;

	// Vegetation Statistics (SCTVegetationType)
	float	numOfEntities	= 0;
	float	numTrees		= 0;
	float	numFoliage		= 0;
	float	numMisc			= 0;

	float	treePercent		= 0; 
	float	foliagePercent	= 0; 
	float	miscPercent		= 0;

	bool	bTree			= false,
			bFoliage		= false,
			bMisc			= false;

	// Check in the vegetation library (mEntities) 
	// what type of entities we have available
	std::vector<SCTVegetationEntity>::iterator it;
	for(it = mEntities.begin(); it != mEntities.end(); it++)
	{
		switch( (*it).mType )
		{
		case SCTVegetationEntity::eTree:
			{
				bTree = true;
			} break;
		case SCTVegetationEntity::eFoliage:
			{
				bFoliage = true;
			} break;
		case SCTVegetationEntity::eMisc:
			{
				bMisc = true;
			} break;
		};

		// If all the entities are available, stop the iteration
		if(bTree && bFoliage && bMisc)
			break;
	}

	// No entities in the list... exit
	if(!bTree && !bFoliage && !bMisc)
		return FAIL;

	// Forest generation loop
	for(SUInt32 i = 0; i< config.iterations; i++)
	{
		distance.clear();
		SCTVegetationEntity *vegEntity = NULL;
		Math::Vector3		position;

		// 1. Get random entity type and check against the vegetation statistics
		SUInt32 entityType;

		bool done = false;
		SUInt32 tries	= 0;
		while(!done)
		{
			// Choose a random entity
			entityType	= rand() % SCTVegetationEntity::eMaxTypes;
			
			switch(entityType)
			{
			case SCTVegetationEntity::eTree:
				{
					if( (treePercent < vegConfig.treePercentage) && bTree )
						done = true;

					if( (tries >= 5) && bTree)
						done = true;
				} break;
			case SCTVegetationEntity::eFoliage:
				{
					if( (foliagePercent < vegConfig.foliagePercentage) && bFoliage)
						done = true;

					if( (tries >= 5) && bFoliage)
						done = true;
				} break;
			case SCTVegetationEntity::eMisc:
				{
					if( (miscPercent < vegConfig.miscPercentage) && bMisc)
						done = true;

					if( (tries >= 5) && bMisc)
						done = true;
				} break;
			};
			tries++;
		}

		// 2. Get a random entity of that type from the mEntities map
		done = false;
		while(!done)
		{
			SUInt32 entityIndex	= rand() % mEntities.size();
			if(mEntities[entityIndex].mType == entityType)
			{
				vegEntity	= &mEntities[entityIndex];
				done		= true;
			}
		}

		// 3. Generate random position for this entity
		done			= false;
		bool bOutside	= true;		// indicates if the current position is outside all the radiuses
		tries	= 0;
		while(!done)
		{
			float posRange	= float(vegConfig.size / 2);
			position		= Math::Vector3(Math::GetRandom(-posRange, posRange), 0, Math::GetRandom(-posRange, posRange));

			// Iterate through entityPositions
			std::vector<entry>::iterator posIt;
			for(posIt = entityPositions.begin(); posIt != entityPositions.end(); posIt++)
			{
				// For this entity, check if related
				if(IsRelated(vegEntity->mID, (*posIt).id))
				{
					// Do a max radius check
					float distance = (position - (*posIt).position).Length();
					
					if( !(distance >= vegEntity->mRadiusRange.max) )
						bOutside = false;
				}
				else
				{
					// Do a min radius check
					float distance = (position - (*posIt).position).Length();

					if( !(distance >= vegEntity->mRadiusRange.min) )
						bOutside = false;
				}
			}

			done = bOutside;

			tries++;
			if(tries > 50)
				break;
		}

		if(tries > 50)
			continue;

		// 4. Add this entity to the temp map
		entry	e;
		e.id		= vegEntity->mID;
		e.position	= position;

		entityPositions.push_back(e);

		// And to the scene ... 
		position.y	= 5000.0f;

		// Cast a ray to determine the height of the terrain at this point
		Math::Ray vegRay(position, Math::Vector3(0, -1.0f, 0));

		if(sceneMngr->CheckIntersection(terrain, vegRay, distance))
			position = vegRay.GetPoint(distance[0]);

		// Add the y offset
		position.y	+= vegEntity->mYOffset;

		// Create the scene node
		//Math::Matrix4 ltwMatrix;
		//ltwMatrix.SetTranslation(position);

		// TODO: Random scale
		// TODO: Random orientation
		// TODO: entity individual 'y offset'

		// Node name
		std::string name = vegEntity->mName;
		name.append("_");
		name.append(Core::SCTStringHelper::getSingleton().numberToString(i));
		name.append("_");
		name.append(Core::SCTStringHelper::getSingleton().numberToString(vegEntity->mID));

		Scene::SCTSceneNode *vegSceneNode = sceneMngr->GetRoot()->AddChild(vegEntity->mpMesh, name.c_str());
		//vegSceneNode->SetLocalToWorldMatrix(ltwMatrix);
		vegSceneNode->SetPosition(position);
		vegSceneNode->SetRotation(Math::GetRandom(0, float(Math::PI) * 2) , 0, 0);

		// Calculate the statistics
		numOfEntities++;

		switch(entityType)
		{
		case SCTVegetationEntity::eTree:
			{
				numTrees++;
			} break;
		case SCTVegetationEntity::eFoliage:
			{
				numFoliage++;
			} break;
		case SCTVegetationEntity::eMisc:
			{
				numMisc++;
			} break;
		};

		treePercent		= numTrees / numOfEntities; 
		foliagePercent	= numFoliage / numOfEntities; 
		miscPercent		= numMisc / numOfEntities;
	}

	std::cout<< "\n------- Vegetation statistics -------\n";
	std::cout<< "Number of vegetation entities: " << numOfEntities << "\n";
	std::cout<< "Trees: " << numTrees << ", [" << treePercent << "%]\n";
	std::cout<< "Foliage: " << numFoliage << ", [" << foliagePercent << "%]\n";
	std::cout<< "Misc: " << numMisc << ", [" << miscPercent << "%]\n";
	std::cout<< "---------------------------------------\n\n";

	return OK;
}

SCTReturn SCTVegetation::CreateVegetationEntity(const std::string &name, Resource::SCTMesh *mesh, float minRange, float maxRange, float yOffset, SCTVegetationEntity::SCTVegetationType type)
{
	SCTVegetationEntity	entity;

	SUInt32 id;
	do
	{
		id	= rand() % 1000;
	} while(CheckDuplicateID(id));

	entity.mID				= id;
	entity.mName			= name;
	entity.mRadiusRange.min	= minRange;
	entity.mRadiusRange.max	= maxRange;
	entity.mpMesh			= mesh;
	entity.mType			= type;
	entity.mYOffset			= yOffset;

	// Add to the vegetation entity map
	mEntities.push_back(entity);

	return OK;
}

SCTReturn SCTVegetation::AddRelationship(const std::string &entity_1, const std::string &entity_2)
{
	SUInt32 entity_1_ID,
			entity_2_ID;

	// Find entity_1 & entity_2 in mEntities and get id
	std::vector<SCTVegetationEntity>::iterator it;
	for(it = mEntities.begin(); it != mEntities.end(); it++)
	{
		// Get Entity 1 ID
		if(std::strcmp((*it).mName.c_str(), entity_1.c_str()) == 0)
			entity_1_ID	= (*it).mID;

		// Get Entity 1 ID
		if(std::strcmp((*it).mName.c_str(), entity_2.c_str()) == 0)
			entity_2_ID	= (*it).mID;
	}

	SCTEntityRelationship relationship;

	relationship.first	= entity_1_ID;
	relationship.second	= entity_2_ID;

	mRelationships.push_back(relationship);

	return OK;
}

// 
bool SCTVegetation::CheckDuplicateID(SUInt32 id)
{
	std::vector<SCTVegetationEntity>::iterator it;

	// Iterate through the mEntities map and check if this id exists
	for(it = mEntities.begin(); it != mEntities.end(); it++)
	{
		if((*it).mID == id)
			return true;
	}

	return false;
}

bool SCTVegetation::IsRelated(SUInt32 id_1, SUInt32 id_2)
{
	std::vector<SCTEntityRelationship>::iterator	it;

	for(it = mRelationships.begin(); it != mRelationships.end(); it++)
	{
		if( ((*it).first == id_1) && ((*it).second == id_2) )
			return true;
		if( ((*it).second == id_1) && ((*it).first == id_2) )
			return true;
	}

	return false;
}

}	// End of PCG Namespace
}	// End of SCT Namepspace