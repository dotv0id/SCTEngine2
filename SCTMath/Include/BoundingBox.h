/**
----------------------------------------------------------------------------
	@File:			BoundingBox.h
   
	System:         SCT Math Library
	Component Name:	Bounding Box
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

#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H


#include <SCTMathDefines.h>
#include <Vector3.h>
//#include <Ray.h>


namespace Math
{
	// Foreward declarations
	class Ray;

	/// BoundingBox
	//  ...
	class BoundingBox
	{
	public:
		BoundingBox() : mMin (Vector3(0, 0, 0)), mMax (Vector3(0, 0, 0)), mCenter (Vector3(0, 0, 0)) {};
		BoundingBox(const Vector3 &min, const Vector3 &max) : mMin (min), mMax (max)
		{
			mCenter.x = (mMin.x + mMax.x) * 0.5f;
			mCenter.y = (mMin.y + mMax.y) * 0.5f;
			mCenter.z = (mMin.z + mMax.z) * 0.5f;
		}

		BoundingBox(const BoundingBox &aabb) : mMin (aabb.mMin), mMax (aabb.mMax), mCenter (aabb.mCenter) {};

		void	Calculate(Vector3 *vertices, unsigned int numVertices);
		void	Translate(const Vector3 &translation);

		bool	CheckIntersection(const BoundingBox &aabb);
		bool	CheckIntersection(const Vector3 &vector);

		void operator = (BoundingBox b)
		{
			mMin		= b.mMin;
			mMax		= b.mMax;
			mCenter		= b.mCenter;
		}

		Vector3		mMin, 
					mMax, 
					mCenter;
	};
}	// End of Math Namespace


#endif