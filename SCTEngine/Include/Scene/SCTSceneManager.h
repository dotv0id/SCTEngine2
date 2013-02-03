/**
----------------------------------------------------------------------------
	@File:			SCTSceneManager.h
   
	System:         SCT Game Engine
	Component Name: SCTSceneManager
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

#ifndef SCT_SCENE_MANAGER_H
#define SCT_SCENE_MANAGER_H


#include <SCTMath.h>
#include <Scene\SCTSceneNode.h>
#include <SCTSingleton.h>
#include <vector>


namespace SCT
{
	namespace Scene
	{
		// Foreward class declarations
		class SCTCamera;

		/// Class SCTSceneManager
		//  ...
		class SCTSceneManager : public Singleton<SCTSceneManager>
		{
		public:
			SCTSceneManager();
			~SCTSceneManager();

			SCTReturn				Initilize();
			SCTReturn				Update(float elapsedTime);
			SCTReturn				Shutdown();

			inline void				EnableFrustumCulling(bool mode) { mbFrustumCulling	= mode; }

			// Check intersection of Ray against a scene node
			// returns true; if intersection deteccted & distance from ray origin
			SUInt32					CheckIntersection(SCTSceneNode *node, const Math::Ray &ray, std::vector<float> &distance, bool stopAtFirst = true);

			// Get, Set functions
			SCTSceneNode*			GetSceneNode(const char *name);

			// Returns a list with all the nodes in the scene having "name" in their name
			SUInt32					GetSceneNodes(const char *name, std::vector<SCTSceneNode*> &nodeList);

			inline SCTSceneNode*	GetRoot() { return mRootNode; }			
			inline SUInt32			GetSceneNodesCount() { return mNodeCount; }
			inline void				SetCamera(SCTCamera *camera) { mpCamera = camera; }

		private:
			SCTSceneNode			*mRootNode;
			SUInt32					mNodeCount;
			SCTCamera				*mpCamera;

			bool					mbFrustumCulling;

		};
	}	// End of Namespace Scene

	// Singlenton
	template<> Scene::SCTSceneManager* Singleton<Scene::SCTSceneManager>::ms_Singleton = 0;
}	// End of Namespace SCT


#endif