/**
----------------------------------------------------------------------------
	@File:			SCTMathDebug.h
   
	System:         SCTMath Library 
	Component Name: SCTMath Debug functions
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


#ifndef SCT_MATH_DEBUG_H
#define SCT_MATH_DEBUG_H

#include <SCTMath.h>

namespace Math
{
	/// Debug Functions
	void DumpVector3(const Vector3 &vec);
	void DumpMatrix4(const Matrix4 &m);

}	// End of Math Namespace

#endif