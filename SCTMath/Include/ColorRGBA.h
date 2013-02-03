/**
----------------------------------------------------------------------------
	@File:			Color.h
   
	System:         SCT Math Library
	Component Name:	ColorRGBA
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

#ifndef COLOR_RGBA_H
#define COLOR_RGBA_H


#include "SCTMathDefines.h"

namespace Math
{
	/// Class Color
	/// ...
	class ColorRGBA
	{
	public:
		inline ColorRGBA() : r(0), g(0), b(0), a(1.0f) {}
		inline ColorRGBA(float fR, float fG, float fB, float fA) : r(fR), g(fG), b(fB), a(fA) {}
		inline ColorRGBA(const ColorRGBA &rC) : r(rC.r), g(rC.g), b(rC.b), a(rC.a) {}

		~ColorRGBA();

		float		r, 
					g, 
					b, 
					a;
	};
}	// End of Math Namespace

#endif