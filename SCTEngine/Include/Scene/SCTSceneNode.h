/**
----------------------------------------------------------------------------
	@File:			SCTSceneNode.h
   
	System:         SCT Game Engine
	Component Name: SCTSceneNode
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

#ifndef SCT_SCENE_NODE_H
#define SCT_SCENE_NODE_H


#include <SCT.h>
#include <SCTMath.h>
#include <SCTMathDebug.h>
#include <list>


namespace SCT
{
	// Foreward class declarations
	class SCTEngine;

	namespace Resource
	{
		class SCTMesh;
	}
	namespace Scene
	{
		class SCTSceneManager;
	}

	namespace Scene
	{
		/// Class SCTSceneNode
		//  ...
		class SCTSceneNode
		{
		public:
			SCTSceneNode();
			SCTSceneNode(const std::string &name);
			~SCTSceneNode();

			// Node data type
			enum SCTNodeDataType
			{
				eMesh	= 0,
				eLight
			};

			// Translation Space
			enum SCTTranslationSpace
			{
				eWorld = 0,
				eLocal,
				eParent
			};

			SCTReturn					Update(float elapsedTime);			

			inline SCTSceneNode*		AddChild(Resource::SCTMesh *mesh, const std::string &name = "")
			{
				SCTSceneNode *newNode	= new SCTSceneNode(name);
				
				newNode->SetData(mesh, eMesh);
				newNode->SetParent(this);

				mChildren.push_back(newNode);

				return newNode;
			}

			// Get, Set functions
			inline SCTSceneNode*		GetChild(const char *name)
			{
				std::list<SCTSceneNode*>::iterator it;
				for(it = mChildren.begin(); it != mChildren.end(); it++)
				{
					SCTSceneNode* current = *it;
					if(!strcmp(current->mName.c_str(), name))
					{
						return current;
					}
				}

				return NULL;
			}

			inline void					GetData(Resource::SCTMesh **data) { *data	= (Resource::SCTMesh*)mpData; }
			inline SUInt32				GetChildrenCount() { return mChildren.size(); }
			inline Math::Matrix4&		GetLocalToWorldMatrix() { return mLocalToWorldMatrix; }
			inline Math::Vector3		GetPosition() { return mLocalToWorldMatrix.GetTranslation(); }			
			inline bool					GetVisibility() { return mbIsVisible; }

			inline void					SetName(const std::string &name) { mName	= name; }
			inline void					SetData(void* data, SCTNodeDataType dataType) 
			{
				mpData		= data;
				mDataType	= dataType;
			}

			inline void					SetParent(SCTSceneNode *parent) { mParent	= parent; }
			inline void					SetLocalToWorldMatrix(const Math::Matrix4 &localToWorldMatrix)
			{
				mLocalToWorldMatrix	= localToWorldMatrix;
			}

			inline void					SetPosition(const Math::Vector3 &position, SCTTranslationSpace space = eWorld)
			{ 
				mTranslation = position;
				mTranslationSpace	= space;
			}

			inline void					SetScale(const Math::Vector3 &scale) 
			{ 
				mScale = scale; 
			}

			inline void					SetRotation(float yaw, float pitch, float roll, SCTTranslationSpace space = eWorld)
			{
				Math::Quaternion yawRot, pitchRot, rollRot, quatRot;

				quatRot.SetRotationYawPitchRoll(yaw, pitch, roll);
				//yawRot.SetFromAxisAngle(yaw * float(Math::RAD_TO_DEG), Math::Vector3(0, 1.0f, 0));
				//pitchRot.SetFromAxisAngle(pitch * float(Math::RAD_TO_DEG), Math::Vector3(1.0f, 0, 0));
				//rollRot.SetFromAxisAngle(roll * float(Math::RAD_TO_DEG), Math::Vector3(0, 0, 1.0f));

				mOrientation = mOrientation * quatRot;//* yawRot * pitchRot * rollRot;
				mOrientation.Normalize();

				mRotationSpace = space;
			}

			inline void					SetVisibility(bool visibility) { mbIsVisible = visibility; }

		private:
			// --- Private Variables ---
			std::string					mName;
			Math::Matrix4				mLocalToWorldMatrix;

			Math::Quaternion			mOrientation;
			Math::Vector3				mScale,
										mTranslation;

			SCTTranslationSpace			mRotationSpace,
										mTranslationSpace;

			void						*mpData;		// Contains the data of that node [SCTMesh, SCTLight]
			SCTNodeDataType				mDataType;

			bool						mbIsVisible;	// Is this node visible by the user?

			std::list<SCTSceneNode*>	mChildren;
			SCTSceneNode*				mParent;

			// --- Private Functions ---
			void						DeleteAllChildren();

		public:
			friend class SCTEngine;
			friend class SCTSceneManager;
		};
	}	// End of Namespace Scene
}	// End of Namespace SCT


#endif