/**
----------------------------------------------------------------------------
	@File:			SCTPCGTypeDefs.h
   
	System:         SCT Game Engine 
	Component Name:	SCT Procedural Content Generation - type definitions
	Status:         Version 1.0 Release 1 

	Language: C++
   
	License: GNU Public License	[*!]
	Licensed Material - Property of ...
   
	Author: Anastasios 'v0id' Giannakopoulos
	E-Mail: dotvoidd@gmail.com
   
	Description:	Type definitions used in the PCG system
 
	Limitations:
   
	Function:

----------------------------------------------------------------------------
*/


#ifndef _SCT_PCG_TYPEDEFS_H_
#define _SCT_PCG_TYPEDEFS_H_


namespace SCT
{
	namespace PCG
	{
		/* ----------------------------------------------------- *
		 *			Terrain typedefs
		 * ----------------------------------------------------- */
		
		/*!
		 *	Faulting algorithm configuration structure
		 *	Description: 
		 *
		 */
		struct SCTFaulting
		{
			// Number of iterations
			SUInt32		iterations;

			// Used in the eStandardFixed and eSinFixed techniques.
			// defines the displacement of each vertex
			float		displacement;

			// Used in the eStandardRandom, eStandardIncr, eStandardDecr
			// and eSinRandom, eSinIncr, eSinDecr techniques.
			// - Random: The algorithm picks a random number in this range 
			//	and uses it as displacement in the Y axis
			// - Incr: The algorithm uses the min value as a starting point
			// - Decr: The algorithm uses the max value as the starting point
			Math::Range	range;

			// Used in the eStandardIncr, eStandardDecr, eSinIncr and eSinDecr techniques.
			// Defines how much the displacement will increase or decrease in every iteration
			float		step;

			// enum of different Faulting algorithm displacement techniques
			enum FaultingTechnique
			{
				eStandardFixed	= 0,	// Faulting step using a predifined value
				eStandardIncr,			// Faulting step increased in every iterration
				eStandardDecr,			// Faulting step decreased in every iterration
				eStandardRandom,		// Faulting step value randomly chosen from a predefined range
				eSinFixed,				// Using a Sin function
				eSinIncr,				// Using a Sin function
				eSinDecr,				// Using a Sin function
				eSinRandom				// Using a Sin function
			} technique;
		};
		
		/*!
		 *	Diamond Square algorithm configuration structure
		 *	Description: 
		 *
		 */
		struct SCTDiamondSquare
		{
			float		roughness;
		};

		/*!
		 *	Particle Deposition algorithm configuration structure
		 *	Description: 
		 *
		 */
		struct SCTParticleDeposition
		{
			// Jump: defines how many times to change
			// the particle drop point
			SUInt32		jumps;

			// The radius each jump of the algorithm
			// will affect
			float		radius;

			// How often the jump point will be aggitated
			SUInt32		peakWalk;

			// Min and Max particles per jump
			Math::Range	particlesPerJump;
		};

		/*!
		 *	Square 'size*size' Grid configuration
		 *	Description: 
		 *
		 */
		struct SCTGrid
		{
			// Grid size 
			SUInt32			size;
			SUInt32			textureRepetitions;

			// Grid pattern
			enum SCTGridPattern
			{
				eStandard = 0,
				eQuilt
			}				pattern;
		};

		/*!
		 *	Terrain smooth filter configuration
		 *	Description: 
		 *
		 */
		struct SCTSmoothConfig
		{
			// How many times to apply the smooth filter on the terrain
			// Maximum number of passes is 5
			SUInt32		numPasses;

			struct SmoothPass
			{
				float	kFactor;
			};

			// The smoothFactor for each pass
			// k: [0 - 1.0]
			SmoothPass		pass[5];

			enum SmoothType
			{
				eUnspecified = 0,
				eBandFilter,
				eMatrixFilter
			} type;
		};

		/*!
		 *	Elevation map (or displacement factor map) configuration
		 *	Description: 
		 *
		 */
		struct SCTElevationMap
		{
			// Where to load the elevation map from
			char				*filename;

		};

		/*!
		 *	Terrain configuration structure
		 *	Description: 
		 *
		 */
		struct SCTTerrainConfig
		{
			// Grid configuration
			// Generates a grid of size; gridSize * gridSize
			SCTGrid				grid;

			// Min - Max height of the terrain
			Math::Range			height;

			// The displacement factor
			// Will be used if 'useElevationMap' is false
			float				dFactor;

			// Use elevation map to generate the terrain
			bool				useElevationMap;
			SCTElevationMap		elevationMap;

			// Main terrain algorithm: 
			// Diamond Square configuration
			SCTDiamondSquare	dSquareConfig;

			// Post processing of the terrain
			// 1. Apply faulting algorithm to the terrain
			bool				applyFaulting;
			SCTFaulting			faultingConfig;

			// 2. Smooth the terrain
			bool				smoothTerrain;
			SCTSmoothConfig		smoothConfig;
		};


		/* ----------------------------------------------------- *
		 *			Vegetation typedefs
		 * ----------------------------------------------------- */

		struct SCTVegetationConfig
		{
			// Size the vegeatation will be spreaded in
			// usually same size as the terrain
			SUInt32			size;

			// If no vegetation map is used, the Vegetation manager
			// places Vegetation Entities everywhere in the map
			bool		useVegetationMap;
			char*		mapFilename;

			// How many Vegetation entities to placein the scene.
			// Every iteration places one SCTVegetationEntity.
			SUInt32		iterations;

			// Configure the percentages
			// in %
			// eg. treePercentage = 30;
			float		treePercentage,
						foliagePercentage,
						miscPercentage;
		};

	}	// End of PCG namespace
}	// End of SCT namespace

#endif