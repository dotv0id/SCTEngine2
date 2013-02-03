/**
----------------------------------------------------------------------------
	@File:			Ray.h
   
	System:         SCT Math Library
	Component Name:	Ray
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

#ifndef RAY_H
#define RAY_H


#include <SCTMathDefines.h>
#include <Vector3.h>
#include <BoundingBox.h>


namespace Math
{
	/// Class Ray
	//  ...
	class Ray
	{
	public:
		Ray() : mOrigin (Vector3(0, 0, 0)), mDirection (Vector3(0, -1.0f, 0)) {};
		Ray(Vector3 &origin, Vector3 &direction) : mOrigin (origin), mDirection (direction) {};
		~Ray() {};

		// Checks if the ray intersects with a triangle.
		// If an intersection is detected, it returns the distance of that triangle
		// from the ray's origin
		bool				CheckIntersection(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, float &t);
		bool				CheckIntersection(const BoundingBox &aabb, Vector3 &point);
		bool				CheckIntersection(Vector3 &pos, float radius, float &dist);

		// Returns a point on the ray, located 't' distance away from it's origin
		inline	Vector3		GetPoint(float t)
		{
			return Vector3(mOrigin + (mDirection * t));
		}

		// Public variables
		Vector3				mOrigin,
							mDirection;

	};
}	// End of Math Namespace

#endif