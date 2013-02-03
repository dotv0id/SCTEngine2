/**
----------------------------------------------------------------------------
	@File:			SCT.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Defines
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

#ifndef _SCT_H_
#define _SCT_H_

// Standard C / C++ includes
#include <cmath>
#include <cassert>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdarg.h>

#include <vector>
#include <map>

// Include SCT Custom Types
#include <SCTTypes.h>


// We need this define for assimp
#define _CRT_SECURE_NO_WARNINGS


#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")


namespace SCT
{
	// Maximim number of textures we can pass to the shader
	static const SUInt32 MAX_TEXTURES = 5;

	enum SCTTextureType
	{
		eAmbient = 0,
		eDiffuse,
		eSpecular,
		eAlpha,
		eBump
	};

	// Used by the configuration file / dialog
	static const char* CNFG_RENDER_SUBSYSTEM	= "Render Subsystem";
	static const char* CNFG_RENDER_DEVICE		= "Render Device";
	static const char* CFNG_RESOLUTION			= "Resolution";
	static const char* CNFG_FULLSCREEN			= "Fullscreen";
	static const char* CNFG_VSYNC				= "VSync";

	// Resolution
	struct SCTResolution
	{
		SUInt32			width;
		SUInt32			height;
		SUInt32			refreshRate;
	};

	// Render Interface Configuration
	struct SCTConfigRender
	{
		std::string		RenderSubsystem;
		std::string		RenderDevice;
		SCTResolution	Resolution;
		bool			Fullscreen;
		bool			Vsync;
	};

	// SCT Game Engine Error codes
	enum SCTReturn
	{
		FAIL = 0,							// the function failed
		OK,									// the function succeeded 
		OUTOFMEMORY,						// Cannot allocate memory
		REGISTER_CLASS_FAILED = 11,			// the RegisterClassEx win32 function failed
		RENDER_WINDOW_CREATION_FAILED,		// the CreateWindowEx win32 function failed
		CONFIG_DIALOG_CREATION_FAILED,		// the CreateDialogParam win32 function failed
		CREATE_DX10_DEV_FAILED,
		FAILED_TO_OPEN_FILE,
		NULL_POINTER,
		FAILED_TO_ACQUIRE_KEYBOARD_DEVICE,
		FAILED_TO_ACQUIRE_MOUSE_DEVICE,
		RESOURCE_NOT_FOUND,
		FAILED_TO_CREATE_TEXTURE,
		FAILED_TO_CREATE_RENDER_TARGET_TEXTURE,
		NEW_RESOURCE_ALLOCATED,
		CORRUPTED_VERTEX_BUFFER,
		CORRUPTED_INDEX_BUFFER,
		COULD_NOT_OPEN_FILE,
		INVALID_PARAMETER,
		ILLEGAL_OPERATION,
		INVALID_EXTENSION
	};


	template <class T>
	bool ApproxEqual(T A, T B, SInt32 PRECISION) // , int maxUlps
	{  
		float _epsilon = 1 / pow(10.0f, PRECISION);

		T tmp = A - B;
		return (bool)(abs(tmp) <= _epsilon * max(1.0f, abs(A))); //, abs(B)
	}

}	// End of SCT Namespace


#endif	//\ End of Header file