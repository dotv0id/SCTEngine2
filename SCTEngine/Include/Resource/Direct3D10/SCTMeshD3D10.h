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

#ifndef SCT_MESH_D3D10_H
#define SCT_MESH_D3D10_H


#include <Resource\SCTMesh.h>
#include <vector>


namespace SCT
{
	// Foreward Declarations
	namespace Render
	{
	}

	namespace Resource
	{
		/// Class SCTSubmesh Direct3D 10 implementation
		//  ...
		class SCTSubmeshD3D10 : public SCTSubmesh
		{
		public:
			SCTSubmeshD3D10(ID3D10Device *device);
			~SCTSubmeshD3D10();

			SCTReturn		Render();
			SCTReturn		Shutdown();

		protected:
			// Protected Variables
			ID3D10Device	*mpDevice;

			std::string		mSubmeshName;			// Submesh name	

			ID3D10Buffer	*mDXVertexBuffer;		// Dx10 Vertex buffer
			ID3D10Buffer	*mDXIndexBuffer;		// Dx10 Index buffer

			SUInt32			mStride;				// Size of Vertex structure

			// Protected Functions
			void			UpdateVertexBuffer();
			void			UpdateIndexBuffer();
		};
		// End of SCTSubmesh class


		/// Class SCTMesh 
		//  Description: Mesh Direct3D 10 implementation
		//  ...
		class SCTMeshD3D10 : public SCTMesh
		{
		public:
			SCTMeshD3D10(ID3D10Device *device);
			~SCTMeshD3D10();

			SCTReturn					Initialize();
			SCTReturn					Update();
			SCTReturn					RenderSubmesh(SUInt32 index);
			SCTReturn					Shutdown();
			
		protected:
			
			// std::vector<SCTMaterial*> mvMaterials		// Holds the list of materials used for every submesh
			
		public:

		};
	}	// End of Namespace Resource
}	// End of Namespace SCT

#endif