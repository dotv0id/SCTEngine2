/**
----------------------------------------------------------------------------
	@File:			SCTApp.h
   
	System:         SCT Game Engine 
	Component Name:	SCT App
	Status:         Version 1.0 Release 1 

	Language: C++
   
	License: GNU Public License	[*!]
	Licensed Material - Property of ...
   
	Author: Anastasios 'v0id' Giannakopoulos
	E-Mail: dotvoidd@gmail.com
   
	Description:	Sample application for the SCTEngine
					Derives from SCTEngine and overrides it's functions
 
	Limitations:
   
	Function:

----------------------------------------------------------------------------
*/


#ifndef SCT_APP_H
#define SCT_APP_H


#include <SCTEngine.h>
#include <Scene\SCTCamera.h>
#include <PCG\SCTPCG.h>


namespace SCT
{
	namespace Scene
	{
		class SCTSceneNode;
	}

	// Foreward declarations
	namespace Demo
	{
		class Player;
	}

	class SCTApp : public SCTEngine
	{
	public:
		SCTApp();
		~SCTApp();

		SCTReturn							Initialize();
		SCTReturn							HandleInput();
		SCTReturn							Update(float elapsedTime);
		SCTReturn							Shutdown();

		// Different camera types
		enum CameraType
		{
			eDebugFree	= 0,
			ePlayer,
			eNumCameras
		};

		// Crate structure
		struct CrateEntity
		{
			Scene::SCTSceneNode *node;
			SUInt32				shotCount;
		};

	protected:

	private:
		Demo::Player						*mpPlayer;

		PCG::SCTTerrain						*mpTerrainGenerator;
		PCG::SCTVegetation					*mpVegetationGenerator;


		// Which camera to use
		SUInt32								mCameraType;
		Scene::SCTCamera					*mpCameraFree;

		float								mCameraSpeed,
											mCameraSensitivity;

		float								mOrbRadius;

		std::vector<CrateEntity*>			mCrateList;

		Scene::SCTSceneNode				*m1, *m2;

		// --- Private Functions ---
		void								Shoot();

	}; // End of SCTApp class
}	// End of SCT Namespace

#endif