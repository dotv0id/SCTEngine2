/**
----------------------------------------------------------------------------
	@File:			SCTObjLoader.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Obj Loader
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

#ifndef SCT_OBJ_LOADER_H
#define SCT_OBJ_LOADER_H


#include <SCT.h>


namespace SCT
{
	// Foreward declarations
	namespace Render
	{
		struct SCTVertexPosNormTex;
	}

	namespace Tools
	{
		// Used for a hashtable vertex cache when creating the mesh from a .obj file
		struct SCTCacheEntry
		{
			SUInt32			index;
			SCTCacheEntry	*pNext;
		};

		struct FaceType
		{
			int vIndex1, vIndex2, vIndex3;
			int tIndex1, tIndex2, tIndex3;
			int nIndex1, nIndex2, nIndex3;
		};

		struct ModelType
		{
			float x, y, z;
			float tu, tv;
			float nx, ny, nz;
		};

		/// Class SCTObjLoader
		//  ...
		class SCTObjLoader
		{
		public:
			SCTObjLoader();
			~SCTObjLoader();
			
			SCTReturn										LoadModelFromOBJ(const char *filename, 
																				Render::SCTVertexPosNormTex **rVertexBuffer, 
																				SUInt32 **rIndexBuffer,
																				SUInt32 &rVertexCount,
																				SUInt32 &vIndexCount);	// also return material list

		private:
			// Private Variables
			std::vector<SCTCacheEntry*>						mvVertexCache;			// vertex cache
			std::vector<Render::SCTVertexPosNormTex>		mvVertices;				// Vertices
			std::vector<SUInt32>							mvIndices;				// IndicesvVertices
			std::vector<SUInt32>							mvAttributes;			// Attributes
			//std::vector<SCTMaterial>						mvMaterials;			// Materials
			
			std::wstring									mCurrentDir;			// Holds the current working directory

			// Private Functions
			SUInt32											AddVertex(UINT hash, Render::SCTVertexPosNormTex *vertex);
			
			void											ClearLoaderBuffers();
			void											ClearCache();
			
		};
	}	// End of Tools Namespace
}	// End of SCT Namespace

#endif