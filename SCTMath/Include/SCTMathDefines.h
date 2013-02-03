/**
----------------------------------------------------------------------------
	@File:			SCTMathDefines.h
   
	System:         SCTMath Library 
	Component Name: SCTMath Mathematical Constants
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

#ifndef SCT_MATH_DEFINES_H
#define SCT_MATH_DEFINES_H


#include <cmath>
#include <iostream>

namespace Math
{
	/// Mathematical Constants 
	static const double PI						= 3.14159265358979323846;
	static const double HALFPI					= PI / 2;
	static const double TWO_PI					= 2 * PI;
	static const double DEG_TO_RAD				= PI / 180;
	static const double RAD_TO_DEG				= 180 / PI;

	struct Range
	{
		float	min,
				max;
	};

}	// End of Math Namespace

#endif