/**
----------------------------------------------------------------------------
	@File:			Plane.h
   
	System:         SCT Math Library
	Component Name:	Plane
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

#ifndef PLANE_H
#define PLANE_H


#include "SCTMathDefines.h"
#include <Vector3.h>
#include <BoundingBox.h>


namespace Math
{
	/// Class Plane
	//  ...
	class Plane
	{
	public:
		Plane() : a (0), b (0), c (0), d (0) {};
		Plane(float rA, float rB, float rC, float rD) : a (rA), b (rB), c (rC), d (rD) {};
		Plane(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3) 
		{
			Vector3 e1, e2, n;

			e1 = v2 - v1;
			e2 = v3 - v1;

			n = e1.Cross(e2);
			n.Normalize();

			a = n.x;
			b = n.y;
			c = n.z;
			d = - (a * v1.x + b * v1.y + c * v1.z);

			mPointOnPlane = v1;
		}

		Plane(const Plane &plane) : a (plane.a), b (plane.b), c (plane.c), d (plane.d) {};
		

		// Intersection checks
		bool				CheckIntersectionAABB(const BoundingBox &aabb);

		// Get, Set functions
		inline Vector3		GetNormal()
		{
			return Vector3(a, b, c);
		}

		inline void			SetNormal(const Vector3 &normal)
		{
			a	= normal.x;
			b	= normal.y;
			c	= normal.z;
		}

		inline float		GetDistance(const Vector3 &v)
		{
			return a * v.x + b * v.y + c * v.z + d;
		}

		inline float		GetDistance(float x, float y, float z)
		{
			return a * x + b * y + c * z + d;
		}

		// Operator overloading
		inline bool			operator == (const Plane &plane)
		{
			return (a == plane.a && b == plane.b && c == plane.c && d == plane.d);
		}

		inline void			operator = (const Plane &plane)
		{
			a	= plane.a;
			b	= plane.b;
			c	= plane.c;
			d	= plane.d;
		}

		float		a,
					b, 
					c, 
					d;
		Vector3		mPointOnPlane;
	};
}	// End of Math Namespace

#endif