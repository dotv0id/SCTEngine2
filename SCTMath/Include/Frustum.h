/**
----------------------------------------------------------------------------
	@File:			Frustum.h
   
	System:         SCT Math Library
	Component Name:	Frustum
	Status:         Version 1.0 Release 1 

	Language: C++
   
	Author: Anastasios 'v0id' Giannakopoulos
	E-Mail: dotvoidd@gmail.com
   
	Description:	
 
	Limitations:
   
	Function:

----------------------------------------------------------------------------
*/

#ifndef FRUSTUM_H
#define FRUSTUM_H


#include "SCTMathDefines.h"
#include <Vector3.h>
#include <Matrix4.h>
#include <BoundingBox.h>
#include <Plane.h>
#include <vector>


namespace Math
{
	/// Class Frustrum
	//  ...
	class Frustum
	{
	public:
		Frustum() {};
		Frustum(const Frustum &frustrum) {};

		void			Calculate(float angle, float ratio, float near, float far, Vector3 &camPos, Vector3 &lookAt, Vector3 &up);
		void			Calculate(float depth, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix);

		// Checks if an aabb is inside the frustum
		bool			IsVisiblePoint(float x, float y, float z);
		bool			IsVisibleAABB(const BoundingBox &aabb);

		inline Plane&	GetPlane(int index)
		{
			return mFrustum[index];
		}

	private:
		std::vector<Plane> mFrustum;
	};

}	// End of Math Namespace

#endif