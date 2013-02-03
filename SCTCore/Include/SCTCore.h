/**
----------------------------------------------------------------------------
	@File:			SCTCore.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Core Library
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


#ifndef SCT_CORE_H
#define SCT_CORE_H


// SCT Core 
#include <Core\SCTWindows.h>
#include <Core\SCTStringHelper.h>
#include <Core\SCTLogManager.h>
#include <Core\SCTFileManager.h>
#include <Core\SCTTime.h>
#include <Core\SCTStatistics.h>
#include <Core\SCTImageLoader.h>


namespace SCT
{
	namespace Core
	{
		// Initilize the core utilities
		SCTReturn InitializeCore();

		// Shutdown the core Utilities
		SCTReturn ShutdownCore();

	}	// End of Core Namespace

	void SCTPrintD(const char* format, ...);

}	// End of SCT Namespace

#endif