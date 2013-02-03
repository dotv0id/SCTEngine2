/**
----------------------------------------------------------------------------
	@File:			SCTTerrain.h
   
	System:         SCT Game Engine 
	Component Name:	SCT P.C.G. Terrain 
	Status:         Version 1.0 Release 1 

	Language: C++
   
	License: GNU Public License	[*!]
	Licensed Material - Property of ...
   
	Author: Anastasios 'v0id' Giannakopoulos
	E-Mail: dotvoidd@gmail.com
   
	Description:	Terrain PCG...
 
	Limitations:
   
	Function:

----------------------------------------------------------------------------
*/

#ifndef _SCT_TERRAIN_H_
#define _SCT_TERRAIN_H_


#include <SCT.h>
#include <SCTSingleton.h>
#include <SCTMath.h>
//#include <SCTCore.h>
#include <PCG\SCTPCGTypeDefs.h>
#include <Render\SCTVertexDefs.h>


namespace SCT
{
	// Foreward declarations
	namespace Resource
	{
		class SCTSubmesh;
	}

	namespace PCG
	{
		/// Class SCTTerrain
		//  ...
		class SCTTerrain : public Singleton<SCTTerrain>
		{
		public:
			SCTTerrain();
			~SCTTerrain();

			SCTReturn			Initialize();
			SCTReturn			Shutdown();

			// Generates a terrain procedurally
			SCTReturn			GenerateTerrain(const SCTTerrainConfig &config);

			// Terrain Generation algorithms
			SCTReturn			ApplyFaulting(	Render::SCTVertexPosTangBiNormTex	*vertices, 
												SUInt32 vertexCount,
												const SCTFaulting &config, 
												const Math::Range &height,
												float dFactor = 0);

			SCTReturn			ApplyParticleDeposition(Render::SCTVertexPosTangBiNormTex	*vertices, 
														SUInt32 vertexCount,
														const SCTParticleDeposition &config,
														const Math::Range &height,
														float dFactor = 0);
			
			SCTReturn			ApplyDiamondSquare(	Render::SCTVertexPosTangBiNormTex	*vertices, 
													SUInt32 vertexCount,
													const SCTDiamondSquare &config,
													const Math::Range &height,
													float dFactor = 0);

			//SCTReturn			ApplyNoise();
			//SCTReturn			ApplyHeightMap();

			// Get, Set functions
			inline void			SetTargetMesh(Resource::SCTSubmesh *inMesh)
			{
				mpMesh	= inMesh;
			}

			// Returns the height of the terrain at a specific point,
			float				GetTerrainHeight(float x, float z, float tileSpacing = 1.0f);


		protected:

		private:
			// --- Private Variables ---
			Resource::SCTSubmesh	*mpMesh;			// The terget Mesh for the System
			std::vector<float>		mElevationMap;

			std::vector<float>		mHeightmap;			// This is where the final heightmap is stored
			SUInt32					mHeightmapSize;		// Size of the heightmap (width * width )
			SUInt32					mHeightmapWidth;

			// --- Private Functions ---
			SCTReturn				GenerateGrid(const SCTGrid &grid);
			SCTReturn				GenerateIndices(SUInt32 size, SCTGrid::SCTGridPattern pattern, SUInt32 **indexBuffer, SUInt32 &indexCount);
			SCTReturn				CalculateNormals();
			SCTReturn				CalculateTangentsBinormals();

			// Particle deposition functions
			bool					DropParticle(	SUInt32 currentIndex,
													SUInt32 rootIndex,
													float radius,
													float dFactor,
													Render::SCTVertexPosTangBiNormTex	*vertices,
													SInt32	gridSize);

			// Elevation Map functions
			void					ProcessElevationMap(const std::vector<ColorRGB> &inRawImageData, 
														const SCTElevationMap &config,
														std::vector<float> &outElevationMap);

			// Smoothing functions
			void					SmoothTerrainBandFilter(Render::SCTVertexPosTangBiNormTex	*vertices, SUInt32 vertexCount, float k);
			void					SmoothTerrainMatrixFilter(Render::SCTVertexPosTangBiNormTex *vertices, SUInt32 vertexCount, float k);
		};


		/// Class SCTNoiseGenerator
		//  ...
		class SCTNoiseGenerator
		{
		public:
			SCTNoiseGenerator();
			~SCTNoiseGenerator();

			SCTReturn		Initialize();
			SCTReturn		Shutdown();

		private:

		};

	}	// End of PCG Namespace

	// Singlenton
	template<> PCG::SCTTerrain* Singleton<PCG::SCTTerrain>::ms_Singleton = 0;

}	// End of SCT Namespace


#endif