/**
----------------------------------------------------------------------------
	@File:			SCTBase.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Base Class - unused for now
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

#ifndef SCT_BASE_H
#define SCT_BASE_H

#include "SCT.h"

namespace SCT
{
	//	Class name:	 SCTBase
	//	Description: 
	//
	class SCTBase
	{
	public:
		SCTBase();
		~SCTBase();

		virtual SCTReturn				Initialize();
		virtual SCTReturn				Shutdown();

	protected:

	private:
		bool							m_bIsInitialized;

	};
}	// End of SCT namespace

#endif