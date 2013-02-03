/**
----------------------------------------------------------------------------
	@File:			Player.h
   
	System:         SCT Demo scene 
	Component Name: Player class
	Status:         Version 1.0 Release 1 

	Language: C++
   
	License: GNU Public License	[*!]
	Licensed Material - Property of ...
   
	Author: Anastasios 'v0id' Giannakopoulos
	E-Mail: dotvoidd@gmail.com
   
	Description:	Class to control the player
 
	Limitations:
   
	Function:

----------------------------------------------------------------------------
*/


#ifndef PLAYER_H
#define PLAYER_H


#include <SCT.h>
#include <SCTMath.h>


namespace SCT
{
	// Foreward declarations
	namespace Scene
	{
		class SCTCamera;
		class SCTSceneNode;
	}

	namespace Demo
	{	
		// Description: Class to control the player in the world
		// ...
		class Player
		{
		public:
			Player();
			~Player();
	
			SCTReturn					Initialize();
			SCTReturn					ProcessInput(float elapsedTime);
			SCTReturn					Update();
			SCTReturn					Shutdown();


			// Get, Set functions
			inline Scene::SCTCamera*	GetCamera()
			{
				return mpCamera;
			}

			inline Math::Vector3		GetPosition()
			{
				return mPosition;
			}

		private:
			Scene::SCTCamera			*mpCamera;
			float						mCameraHeightOffset;

			Scene::SCTSceneNode			*mpPlayerNode,
										*mpWeaponNode;

			// Player properties
			Math::Vector3				mPosition,
										mCameraOffset,		// Camera offset from the terrain
										mWeaponOffset,		// Weapon position relative to the camera
										mLastIn;

			float						mMoveSpeed,
										mSensitivity;

			bool						mbAccelerate;

			// Jump
			bool						mbJump;				// indicates whether in jump simulation on
			float						mJumpInit,
										mJumpMax;

			// Crouch
			float						mCrouchLimit,
										mCrouchInit;

			float						mCurrentPitch,
										mPitchLimit;

		};
	}	// End of Demo Namespace
}	// End of SCT Namespace

#endif