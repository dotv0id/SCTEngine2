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

#ifndef SCT_VERTEX_DEFS_H
#define SCT_VERTEX_DEFS_H


#include <SCTMath.h>


namespace SCT
{
	namespace Render
	{
		/// Typedefs used by the Rendering Interface
		//  ...	

		// Vertex Types
		enum SCTVertexType
		{
			ePosition	= 0,
			ePosColor,
			ePosTexture,
			ePosNormTex,
			ePosTangNormTex,
			ePosTangBiNormTex,
			eMAXTYPES,
		};

		// Vertex: Position
		struct SCTVertexPos
		{
			Math::Vector3		position;
		};

		// Vertex: Position, Color
		struct SCTVertexPosColor
		{
			Math::Vector3		position;
			Math::ColorRGBA		color;
		};

		// Vertex: Position, Texture
		struct SCTVertexPosTexture
		{
			Math::Vector3		position;
			Math::Vector2		texture;
		};

		// Vertex: Position, Normal, Texture
		struct SCTVertexPosNormTex
		{
			Math::Vector3		position;
			Math::Vector3		normal;
			Math::Vector2		texture;
		};

		// Vertex: Position, Tangent, Normal, Texture
		struct SCTVertexPosTangNormTex
		{
			Math::Vector3		position;
			Math::Vector3		tangent;
			Math::Vector3		normal;
			Math::Vector2		texture;
		};

		// Vertex: Position, Tangent, Normal, Texture
		struct SCTVertexPosTangBiNormTex
		{
			Math::Vector3		position;
			Math::Vector3		tangent;
			Math::Vector3		binormal;
			Math::Vector3		normal;
			Math::Vector2		texture;
		};
		
	}	// End of Namespace Render
}

#endif