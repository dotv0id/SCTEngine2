/**
----------------------------------------------------------------------------
	@File:			Vector2.h
   
	System:         SCT Math Library 
	Component Name: SCTMath Vector2
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

#ifndef VECTOR2_H
#define VECTOR2_H


#include <SCTMathDefines.h>

namespace Math
{
	/// Class Vector2
	/// ...
	class Vector2
	{
	public:
		inline	Vector2() : x (0), y (0){}
		inline	Vector2(float fX, float fY) : x (fX), y (fY) {}
		inline	Vector2(const Vector2 &vec) : x (vec.x), y (vec.y) {}

		~Vector2();

		inline Vector2			operator - (const Vector2 &vec2) const
		{
			return Vector2(x - vec2.x, y - vec2.y);
		}

		/// ...
		inline float			LengthSquared() const
		{
			return (float)(x*x + y*y);
		}

		inline float			Length() const
		{
			return (float)sqrt(LengthSquared());
		}		

		float	x,
				y;
	};
}	// End of Namespace Math

#endif