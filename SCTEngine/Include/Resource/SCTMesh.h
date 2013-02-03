/**
----------------------------------------------------------------------------
	@File:			SCTMesh.h
   
	System:         SCT Game Engine
	Component Name: SCTMesh
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


#ifndef SCT_MESH_H
#define SCT_MESH_H


#include <Render\SCTRenderDefines.h>
#include <Render\SCTVertexDefs.h>


namespace SCT
{
	// Foreward declaration of classes
	namespace Resource
	{
		class SCTMaterial;
	}

	namespace Resource
	{
		/// Class SCTSubmesh
		//  ...
		class SCTSubmesh
		{
		public:
			virtual							~SCTSubmesh() {};

			virtual SCTReturn				Render() = 0;
			virtual SCTReturn				Shutdown() = 0;

			// Get, Set Functions
			inline void						SetVertexBuffer(void *vertexBuffer, SUInt32 vertexCount, Render::SCTVertexType vertexType)
			{
				mVertexType		= vertexType;

				mVertexBuffer	= (void*)vertexBuffer;				
				mVertexCount	= vertexCount;

				UpdateVertexBuffer();
			}

			inline void						SetIndexBuffer(SUInt32 *indexBuffer, SUInt32 indexCount)
			{
				mIndexBuffer	= indexBuffer;
				mIndexCount		= indexCount;

				UpdateIndexBuffer();
			}

			inline void						SetAABB(const Math::BoundingBox &aabb)
			{
				mAABB = aabb;
			}

			inline void						GetVertexBuffer(Render::SCTVertexPosNormTex **vertexBuffer, SUInt32 &vertexCount)
			{
				*vertexBuffer	= (Render::SCTVertexPosNormTex*)mVertexBuffer;
				vertexCount		= mVertexCount;
				//vertexType		= mVertexType;
			}

			inline void						GetVertexBuffer(Render::SCTVertexPosTangBiNormTex **vertexBuffer, SUInt32 &vertexCount)
			{
				*vertexBuffer	= (Render::SCTVertexPosTangBiNormTex*)mVertexBuffer;
				vertexCount		= mVertexCount;
				//vertexType		= mVertexType;
			}

			inline void						GetIndexBuffer(SUInt32 **indexBuffer, SUInt32 &indexCount)
			{
				*indexBuffer	= mIndexBuffer;
				indexCount		= mIndexCount;
				mPolygonCount	= indexCount / 3;
			}

			inline SUInt32					GetVertexCount() { return mVertexCount; }
			inline SUInt32					GetIndexCount() { return mIndexCount; }
			inline Math::BoundingBox&		GetAABB() { return mAABB; }

			inline Resource::SCTMaterial*	GetMaterial()
			{
				return mpMaterial;
			}

			inline SUInt32					GetMaterialIndex()
			{
				return mMaterialIndex;
			}

			inline SUInt32					GetRenderPriority()
			{
				return mPriority;
			}

			inline void						SetRenderPriority(SUInt32 priority)
			{
				mPriority = priority;
			}

			inline void						SetMaterialIndex(SUInt32 index)
			{
				mMaterialIndex	= index;
			}

			inline void						SetMaterial(Resource::SCTMaterial *material)
			{
				mpMaterial = material;
			}

		protected:
			// Protected Variables
			void							*mVertexBuffer;		// Vertex Buffer
			SUInt32							*mIndexBuffer;		// Index Buffer

			SUInt32							mVertexCount,
											mIndexCount,
											mMaterialIndex,
											mPolygonCount;

			Render::SCTVertexType			mVertexType;

			bool							mbHasVertexBufferChanged,
											mbHasIndexBufferChanged;

			SCTMaterial						*mpMaterial;

			Math::BoundingBox				mAABB;

			// Render priority
			// Renderer first renders nodes with highest priority
			// Range: Lowest: 0 - highest: 2
			SUInt32							mPriority;

			// Protected Functions
			virtual void					UpdateVertexBuffer() = 0;
			virtual void					UpdateIndexBuffer() = 0;
		};
		// End of SCTSubmesh class


		/// Class SCTMesh 
		//  Description: Mesh interface
		//  ...
		class SCTMesh
		{
		public:
			virtual						~SCTMesh() {};

			virtual SCTReturn			Initialize() = 0;
			virtual SCTReturn			Update() = 0;
			virtual SCTReturn			RenderSubmesh(SUInt32 index) = 0;
			virtual SCTReturn			Shutdown() = 0;			

			inline SCTReturn			AddSubmesh(SCTSubmesh *submesh)
			{
				mvSubmeshes.push_back(submesh);
				mNumSubmeshes	= mvSubmeshes.size();

				return OK;
			}

			// Get, Set functions
			inline SCTSubmesh*			GetSubmesh(SUInt32 index)
			{
				if(index <= mvSubmeshes.size())
					return mvSubmeshes[index];

				return NULL;
			}

			inline SUInt32				GetNumberOfSubmeshes() 
			{ 
				return mNumSubmeshes; 
			}

		protected:		
			std::vector<SCTSubmesh*>	mvSubmeshes;		// Holds the submesh list

			SUInt32						mNumSubmeshes;

		public:
			
		};
	}	// End of Namespace Resource
}	// End of Namespace SCT

#endif