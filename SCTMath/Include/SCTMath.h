/**
----------------------------------------------------------------------------
	@File:			SCTMath.h
   
	System:         SCTMath Library 
	Component Name:	SCTMath "Main"
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


#ifndef SCT_MATH_H
#define SCT_MATH_H


#include <Vector2.h>
#include <Vector3.h>
#include <Matrix4.h>
#include <Quaternion.h>
#include <Plane.h>
#include <ColorRGBA.h>
#include <Ray.h>
#include <BoundingBox.h>
#include <Frustum.h>


namespace Math
{
	/// Returns a random value from
	//  within a Range r
	float GetRandom(Range r);
	float GetRandom(float min, float max);
	
	/*
	template<typename T>
	T Min(const T& a, const T& b);

	template<typename T>
	T Max(const T& a, const T& b);
	*/
	template<typename T>
	T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

}	// End of Math Namespace

#endif