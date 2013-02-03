#include <SCTApp.h>
#include <Resource\Direct3D10\SCTMeshD3D10.h>
#include <Resource\SCTSkyBox.h>
#include <Resource\SCTMaterial.h>

#include <Render\Direct3D10\SCTRenderInterfaceD3D10.h>

#include <Input\Win32\SCTInputManagerWin32.h>
#include <Input\Win32\SCTKeyboardWin32.h>
#include <Input\Win32\SCTMouseWin32.h>

#include <Scene\SCTSceneNode.h>

//#include <SCTMath.h>
#include <SCTMathDebug.h>

#include "Player.h"


using namespace SCT;


SCTApp::SCTApp()
:	mpPlayer (NULL),
	mpCameraFree (NULL),
	mpTerrainGenerator (NULL),
	mCameraType (eDebugFree),
	mCameraSpeed (50.0f),
	mCameraSensitivity (0.3f)
{
}

SCTApp::~SCTApp()
{
}

SCTReturn SCTApp::Initialize()
{
	SCTEngine::Initialize();

	// Set scene's fog
	SetFog(Math::ColorRGBA(0.5f, 0.5f, 0.5f, 1.0f), 2000.0f, 2000.0f);

	// Initialize the Terrain Generator algorithms
	mpTerrainGenerator	= new PCG::SCTTerrain;
	mpTerrainGenerator->Initialize();

	mpVegetationGenerator	= new PCG::SCTVegetation;
	mpVegetationGenerator->Initialize();

	// Allocate new submesh for the terrain
	Resource::SCTMeshD3D10 *terrainMesh = NULL;
	Resource::SCTResourceManager::getSingleton().GetResource("Terrain_Mesh", &terrainMesh, false);
	
	// Pass this a Submesh to the terrain generator
	// Texture
	Resource::SCTTexture	*grassTexture		= NULL;
	Resource::SCTTexture	*terrainTextureBump = NULL;
	Resource::SCTResourceManager::getSingleton().GetResource("Media\\Textures\\dirt.dds", &grassTexture);
	Resource::SCTResourceManager::getSingleton().GetResource("Media\\Textures\\Rock_big_single_b_normals.dds", &terrainTextureBump);

	Resource::SCTMaterial *terrainMaterial = terrainMesh->GetSubmesh(0)->GetMaterial();
	terrainMaterial->SetTexture(grassTexture, eDiffuse);
	terrainMaterial->SetTexture(terrainTextureBump, eBump);

	mpTerrainGenerator->SetTargetMesh(terrainMesh->GetSubmesh(0));

	/* ----------------------------------------------------- *
		A. Configure the way the terrain will get generated
	 * ----------------------------------------------------- */
	PCG::SCTTerrainConfig terrainConfig;

	// 1. Grid configuration
	terrainConfig.grid.size						= 256;
	terrainConfig.grid.textureRepetitions		= 64;
	terrainConfig.grid.pattern					= PCG::SCTGrid::eQuilt;
	
	// 2. Elevation map configuration
	terrainConfig.useElevationMap				= true;
	terrainConfig.elevationMap.filename			= "Media\\elevationMap_2.jpg";
	
	// 3. Diamond Square Algorithm configuration
	terrainConfig.dSquareConfig.roughness		= 0.7f;

	// 4a. Terrain post-processing: Faulting
	terrainConfig.applyFaulting					= true;
	terrainConfig.faultingConfig.iterations		= 25;
	terrainConfig.faultingConfig.technique		= PCG::SCTFaulting::eStandardFixed;
	terrainConfig.faultingConfig.displacement	= 0.4f;

	// 4b. Terrain post-processing: Smoothing
	terrainConfig.smoothTerrain					= true;
	terrainConfig.smoothConfig.type				= PCG::SCTSmoothConfig::eBandFilter;
	terrainConfig.smoothConfig.numPasses		= 3;
	terrainConfig.smoothConfig.pass[0].kFactor	= 0.9f;
	terrainConfig.smoothConfig.pass[1].kFactor	= 0.8f;
	terrainConfig.smoothConfig.pass[2].kFactor	= 0.7f;
	// ---------- End of Terrain configuration ----------

	// Generate the terrain
	if(!mpTerrainGenerator->GenerateTerrain(terrainConfig))
		Core::SCTLogManager::getSingleton().PrintDebug("Error generating the terrain!");

	// Add the terrain node to the scene manager
	Scene::SCTSceneNode *terrainNode;
	terrainNode = Scene::SCTSceneManager::getSingleton().GetRoot()->AddChild(terrainMesh, "Terrain_Patch_00");
	//terrainNode->SetScale(Math::Vector3(1.0f, 1.0f, 1.0f));

	/* -------------------------------------------------------- *
		B. Configure the way the vegetation will get generated
	 * -------------------------------------------------------- */
	// 1. Load the tree models,
	// in this demo I use 5 different tree, vegetation, rock models to create the forest
	Resource::SCTMeshD3D10 *tree_01				= NULL;
	Resource::SCTMeshD3D10 *tree_02				= NULL;
	Resource::SCTMeshD3D10 *foliage_01			= NULL;
	Resource::SCTMeshD3D10 *foliage_02			= NULL;
	Resource::SCTMeshD3D10 *foliage_03			= NULL;
	Resource::SCTMeshD3D10 *woodContainer_01	= NULL;
	Resource::SCTMeshD3D10 *woodContainer_02	= NULL;

  Resource::SCTMeshD3D10 *cube  = NULL;
  Resource::SCTResourceManager::getSingleton().GetResource("Media\\cube.obj", &cube);



	// Vegetation config
	PCG::SCTVegetationConfig vegConfig;
	vegConfig.size				      = 256;
	vegConfig.iterations		    = 50;
	vegConfig.treePercentage	  = 60;
	vegConfig.foliagePercentage	= 20;
	vegConfig.miscPercentage	  = 20;
	
	Resource::SCTResourceManager::getSingleton().GetResource("Media\\tree_01b.dae", &tree_01);
	Resource::SCTResourceManager::getSingleton().GetResource("Media\\tree_04.dae", &tree_02);
	Resource::SCTResourceManager::getSingleton().GetResource("Media\\cattail.dae", &foliage_01);
	Resource::SCTResourceManager::getSingleton().GetResource("Media\\ArctinumMinus.dae", &foliage_02);
	Resource::SCTResourceManager::getSingleton().GetResource("Media\\OsmundaCinnamonea.dae", &foliage_03);
	Resource::SCTResourceManager::getSingleton().GetResource("Media\\woodContainer_1.dae", &woodContainer_01);

	mpVegetationGenerator->CreateVegetationEntity("Tree01",			tree_01,	0.5f, 10.0f, -0.5f,		PCG::SCTVegetationEntity::eTree);
	mpVegetationGenerator->CreateVegetationEntity("Tree_big",		tree_02,	0.5f, 10.0f, -0.5f,		PCG::SCTVegetationEntity::eTree);
	mpVegetationGenerator->CreateVegetationEntity("cattail",		foliage_01,	0.2f, 0.4f,		0,		PCG::SCTVegetationEntity::eFoliage);
	mpVegetationGenerator->CreateVegetationEntity("ArctinumMinus",	foliage_02,	0.2f, 0.4f,		0,		PCG::SCTVegetationEntity::eFoliage);
	mpVegetationGenerator->CreateVegetationEntity("OsmundaCinnam",	foliage_03,	0.2f, 0.4f,		0,		PCG::SCTVegetationEntity::eFoliage);
	mpVegetationGenerator->CreateVegetationEntity("Crate",	woodContainer_01,	0.2f, 1.0f,	  1.0f,		PCG::SCTVegetationEntity::eMisc);
	
	// Add the relationships between the vegetation entities
	mpVegetationGenerator->AddRelationship("Tree01", "Tree01");
	mpVegetationGenerator->AddRelationship("Tree_big", "Tree_big");
	mpVegetationGenerator->AddRelationship("Tree_big", "Tree01");
	mpVegetationGenerator->AddRelationship("cattail", "cattail");
	mpVegetationGenerator->AddRelationship("ArctinumMinus", "ArctinumMinus");
	mpVegetationGenerator->AddRelationship("OsmundaCinnam", "OsmundaCinnam");
	mpVegetationGenerator->AddRelationship("Crate", "Crate");

	// Generate the forest now
	mpVegetationGenerator->GenerateVegetation(vegConfig, terrainNode);


	// Load scene objects
	// ----------------------------------
	Resource::SCTMeshD3D10 *bigRock = NULL;
	Resource::SCTResourceManager::getSingleton().GetResource("Media\\rock_01.dae", &bigRock);		// Load big rock

	Scene::SCTSceneNode *bigRockNode = NULL;
	bigRockNode = Scene::SCTSceneManager::getSingleton().GetRoot()->AddChild(bigRock, "BigRockOrigin");
	bigRockNode->SetScale(Math::Vector3(0.1f, 0.1f, 0.1f));

	// Position The rock
	std::vector<float> distance;
	Math::Ray rockRay(Math::Vector3(50.0f, 5000.0f, 0), Math::Vector3(0, -1.0f, 0));
	if(Scene::SCTSceneManager::getSingleton().CheckIntersection(terrainNode, rockRay, distance, true))
	{ 
		Math::Vector3 poz = rockRay.GetPoint(distance[0]);
		bigRockNode->SetPosition(poz);
	}

	// Get all the nodes named "Crate", 
	// this will be our shootable object
	std::vector<Scene::SCTSceneNode*> crateList;
	Scene::SCTSceneManager::getSingleton().GetSceneNodes("Crate", crateList);
	
	for(SUInt32 itCrate = 0; itCrate < crateList.size(); itCrate++)
	{
		CrateEntity *crate = new CrateEntity;

		crate->node			= crateList[itCrate];
		crate->shotCount	= 0;

		mCrateList.push_back(crate);
	}

	// --- Load the Skybox -------------
	// ----------------------------------
	Resource::SCTSkyBox	*skybox;
	Resource::SCTResourceManager::getSingleton().GetResource("Media\\Textures\\skybox_1.dds", &skybox, 2000.0f);
	SetSkybox(skybox);
	// ----------------------------------

	// Create the camera
	mpCameraFree	= new Scene::SCTCamera;
	mpCameraFree->SetProjectionMatrix(float(Math::HALFPI/2), float(mConfig.Resolution.width / mConfig.Resolution.height), 1.0f, 2000.0f);
	mpCameraFree->SetPosition(0, 50.0f, -50.0f);
	mpCameraFree->UseFrustum(false);

	// Set this camera as the active render camera
	SetRenderCamera(mpCameraFree);

	// Create & initialize the player controller
	mpPlayer	= new Demo::Player;
	mpPlayer->Initialize();

	return OK;
}

SCTReturn SCTApp::HandleInput()
{
	SCTEngine::HandleInput();

	float elapsedTime	= GetElapsedTime();

	Input::SCTKeyboard	*keyboard	= Input::SCTInputManager::getSingleton().GetKeyboard();
	Input::SCTMouse		*mouse		= Input::SCTInputManager::getSingleton().GetMouse();

	// Choose camera mode
	if(keyboard->IsKeyPressed(Input::SCT_C))
	{
		// Get next camera
		mCameraType = (mCameraType + 1) % eNumCameras;

		switch(mCameraType)
		{
		case eDebugFree:
			{
				SetRenderCamera(mpCameraFree);
				
				// No fog
				SetFog(Math::ColorRGBA(0.0f, 0.0f, 0.0f, 1.0f), 2000.0f, 2000.0f);

				//EnableFrustumCulling(false);

				ShowCursor(true);
			} break; 
		case ePlayer:
			{
				SetRenderCamera(mpPlayer->GetCamera());

				// Add fog to the scene
				SetFog(Math::ColorRGBA(0.5f, 0.5f, 0.5f, 1.0f), 60.0f, 150.0f);

				//EnableFrustumCulling(true);

				ShowCursor(false);
			} break;
		};

		std::cout<<"Camera type: " << mCameraType << "\n";
	}

	keyboard->IsKeyReleased(Input::SCT_C);


	// [0] Move the debug - free camera
	if(mCameraType == SUInt32(eDebugFree))
	{
		// Adjust camera move speed
		if(keyboard->IsKeyPressed(Input::SCT_EQUALS))
			mCameraSpeed	+= 5.0f;
		if(keyboard->IsKeyPressed(Input::SCT_MINUS))
			mCameraSpeed	-= 5.0f;

		keyboard->IsKeyReleased(Input::SCT_EQUALS);
		keyboard->IsKeyReleased(Input::SCT_MINUS);


		// Move the camera
		if(keyboard->GetKeyState(Input::SCT_W))
			mpCameraFree->Move(mCameraSpeed * elapsedTime);
		if(keyboard->GetKeyState(Input::SCT_S))
			mpCameraFree->Move(-mCameraSpeed * elapsedTime);
		
		if(keyboard->GetKeyState(Input::SCT_A))
			mpCameraFree->MovePerpendicular(-mCameraSpeed * elapsedTime, 0);
		if(keyboard->GetKeyState(Input::SCT_D))
			mpCameraFree->MovePerpendicular(mCameraSpeed * elapsedTime, 0);

		// Move the camera up and down
		if(keyboard->GetKeyState(Input::SCT_UP))
			mpCameraFree->MovePerpendicular(0, mCameraSpeed * elapsedTime);
		if(keyboard->GetKeyState(Input::SCT_DOWN))
			mpCameraFree->MovePerpendicular(0, -mCameraSpeed * elapsedTime);


		// Camera rotation with mouse left
		if(mouse->GetMouseState(Input::SCT_MOUSE_LEFT))
		{
			mpCameraFree->RotateYaw(mouse->GetRelativeX() * elapsedTime * mCameraSensitivity);
			mpCameraFree->RotatePitch(mouse->GetRelativeY() * elapsedTime * mCameraSensitivity);
		}

		// Camera Zoom in / out with mouse right
		if(mouse->GetMouseState(Input::SCT_MOUSE_RIGHT))
			mpCameraFree->Move(-mouse->GetRelativeX() * elapsedTime * 50.0f);
	}
	
	// [1] Move the character
	if(mCameraType == SUInt32(ePlayer))
	{
		mpPlayer->ProcessInput(elapsedTime);
		//Math::Vector3 playerPos = mpPlayer->GetPosition();

		//float height = mpTerrainGenerator->GetTerrainHeight(playerPos.x, playerPos.z);

		//Math::DumpVector3(playerPos);
		//std::cout<<"height [GetTerrainHeight]: " << height << ", raycasting: " << playerPos.y << "\n";

		// Get current position
		// using the terrain's heightmap, get the height we should be

		// Shoot when mouse left is preseed
		if(mouse->IsButtonDown(Input::SCT_MOUSE_LEFT))
			Shoot();
		
		mouse->IsButtonUp(Input::SCT_MOUSE_LEFT);
	}

	return OK;
}

SCTReturn SCTApp::Update(float elapsedTime)
{
	SCTEngine::Update(elapsedTime);

	// TEST NODE ROTATION
	Math::Matrix4 worldMtx1, worldMtx2, rotation;
	
	std::vector<CrateEntity*>::iterator itCrate;
	for(itCrate = mCrateList.begin(); itCrate != mCrateList.end(); itCrate++)
	{
		CrateEntity *crate = (*itCrate);
		crate->node->SetRotation(elapsedTime, 0, 0);
	}

	return OK;
}

void SCTApp::Shoot()
{
	// Check if the ray, cast from mouse to -z, intersects with a crate (aabb)
	Math::Matrix4 projMtx = mpCameraFree->GetProjectionMatrix();
	Math::Vector3 origin, direction;
	
	// Compute the vector of the pick ray in screen space
	Math::Vector3 v;
	
	// Always cast a ray from the middle of the screen
	v.x = 0;
    v.y = 0;
    v.z = 1.0f;

	// Get the inverse view matrix
    Math::Matrix4 invViewMatrix	= mpPlayer->GetCamera()->GetViewMatrix();
	invViewMatrix = invViewMatrix.Inverse();

	// Check every crate for intersection
	std::vector<CrateEntity*>::iterator itCrates;
	for(itCrates = mCrateList.begin(); itCrates != mCrateList.end(); itCrates++)
	{
		// Transform the screen space pick ray into 3D space
		direction.x		= v.x * invViewMatrix(1,1) + v.y * invViewMatrix(2,1) + v.z * invViewMatrix(3,1);
		direction.y		= v.x * invViewMatrix(1,2) + v.y * invViewMatrix(2,2) + v.z * invViewMatrix(3,2);
		direction.z		= v.x * invViewMatrix(1,3) + v.y * invViewMatrix(2,3) + v.z * invViewMatrix(3,3);
		origin.x		= invViewMatrix(4,1);
		origin.y		= invViewMatrix(4,2);
		origin.z		= invViewMatrix(4,3);

		Math::Ray pickRay(origin, direction);
		// - (*itCrates)->GetLocalToWorldMatrix().GetTranslation()

		// try
		Math::Vector3 fsPoint;
		Resource::SCTMesh *mesh;
		(*itCrates)->node->GetData(&mesh);

		Math::BoundingBox aabb = mesh->GetSubmesh(0)->GetAABB();

		//pickRay.CheckIntersection(aabb, fsPoint)
		//
		float distance;
		if(pickRay.CheckIntersection((*itCrates)->node->GetLocalToWorldMatrix().GetTranslation(), 2.0f, distance))
		{
			(*itCrates)->shotCount++;
			//if((*itCrates)->shotCount > 3)
				(*itCrates)->node->SetVisibility(false);

			//std::cout << "Shoot Gad damn it!\n";
		}
	}

}

SCTReturn SCTApp::Shutdown()
{
	// Delete all the crates
	std::vector<CrateEntity*>::iterator itCrate;
	for(itCrate = mCrateList.begin(); itCrate != mCrateList.end(); itCrate++)
	{
		delete (*itCrate);
	}

	// Delete the player class
	delete mpPlayer;
	mpPlayer	= NULL;

	// Delete the camera
	delete mpCameraFree;
	mpCameraFree	= NULL;

	// Shutdown PCG classes
	delete mpTerrainGenerator;
	mpTerrainGenerator	= NULL;

	delete mpVegetationGenerator;
	mpVegetationGenerator	= NULL;

	// Shutdown the engine
	SCTEngine::Shutdown();

	return OK;
}