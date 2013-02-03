/**
----------------------------------------------------------------------------
	@File:			SCTPCGFunctions.h
   
	System:         SCT Game Engine 
	Component Name:	SCT Procedural Content Generation
	Status:         Version 1.0 Release 1 

	Language: C++
   
	License: GNU Public License	[*!]
	Licensed Material - Property of ...
   
	Author: Anastasios 'v0id' Giannakopoulos
	E-Mail: dotvoidd@gmail.com
   
	Description:	Common pcg functions
 
	Limitations:
   
	Function:

----------------------------------------------------------------------------
*/


#ifndef _SCT_PCG_FUNCTIONS_H_
#define _SCT_PCG_FUNCTIONS_H_


#include <SCT.h>
#include <Render\SCTVertexDefs.h>


namespace SCT
{
	namespace PCG
	{
		// [!] Taken from Frank Luna's Framework
		// Name: BuildGeoSphere
		// Desc: Function approximates a sphere by tesselating an icosahedron.
		void BuildGeoSphere(SUInt32 numSubdivisions, float radius, std::vector<Math::Vector3> &outVertices, std::vector<SUInt32> &outIndices);
		
		// [!] Taken from Frank Luna's Framework
		// Name: Subdivide
		// Desc: Function subdivides every input triangle into four triangles of equal area.
		void Subdivide(std::vector<Math::Vector3> &outVertices, std::vector<SUInt32> &outIndices);

	}	// End of PCG Namespace
}	// End of SCT Namespace


#endif