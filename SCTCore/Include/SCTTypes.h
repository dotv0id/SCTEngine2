/**
----------------------------------------------------------------------------
	@File:			SCTTypes.h
   
	System:         SCTGame Engine 
	Component Name:	SCT typedefs
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

#ifndef SCT_TYPES_H
#define SCT_TYPES_H

namespace SCT
{
	typedef __int8				SInt8;		// 8 bit integer
	typedef __int16				SInt16;		// 16 bit integer
	typedef __int32				SInt32;		// 32 bit integer
	typedef __int64				SInt64;		// 64 bit integer

	typedef unsigned __int8		SUInt8;		// 8 bit unsigned integer
	typedef unsigned __int16	SUInt16;	// 16 bit unsigned integer
	typedef unsigned __int32	SUInt32;	// 32 bit unsigned integer
	typedef unsigned __int64	SUInt64;	// 64 bit unsigned integer

	/// Description: SCTFontType struct
	//  Holds the font data from dile
	struct SCTFontType
	{
		float	left, 
				right;
		int		size;
	};

	// Color RGB
	struct ColorRGB
	{
		float r, g, b;
	};
}

#endif