/**
----------------------------------------------------------------------------
	@File:			SCTPlatform.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Platform
	Status:         Version 1.0 Release 1 

	Language: C++
   
	License: GNU Public License	[*!]
	Licensed Material - Property of ...
   
	Author: Anastasios 'v0id' Giannakopoulos
	E-Mail: dotvoidd@gmail.com
   
	Description:	Includes platform specific configuration of the
					SCTEngine
 
	Limitations:
   
	Function:

----------------------------------------------------------------------------
*/


#ifndef SCT_PLATFORM_H
#define SCT_PLATFORM_H


// Use Direct Input
#define SCT_USE_DIRECT_INPUT
#define SCT_USE_DIRECTX10


#ifdef SCT_USE_DIRECT_INPUT
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#endif



// Includes for Windows
#ifdef _WIN32
	#include <windows.h>
	#include <CommCtrl.h>
	#include <D3DX10.h>
	#include <dxerr.h>

	// Visual leak detector to detect memory leaks
	#if defined(DEBUG) || defined(_DEBUG)
	#include <vld.h>
	#pragma comment(lib, "vld.lib")
	#endif

	// Include the resource file for our configuration dialog
	#include "resource.h"

	// We are compiling for Windows XP and above
	#ifndef _WIN32_WINNT
	#define _WIN32_WINNT   0x0600 // Vista
	#endif

	// Speed up the build proccess
	#define WIN32_LEAN_AND_MEAN

	// To get common control 6 visual styles
	#pragma comment(linker, \
					"\"/manifestdependency:type='Win32' "\
					"name='Microsoft.Windows.Common-Controls' "\
					"version='6.0.0.0' "\
					"processorArchitecture='*' "\
					"publicKeyToken='6595b64144ccf1df' "\
					"language='*'\"")

	// to call InitCommonControls(...)
	#pragma comment(lib, "ComCtl32.lib")

	// DIRECTX 10 LIBS
	#pragma comment(lib, "d3d10.lib")
	#pragma comment(lib, "dxerr.lib")
	#pragma comment(lib, "dxgi.lib")
	#pragma comment(lib, "dxguid.lib")

	#if defined(DEBUG) || defined(_DEBUG)
		#pragma comment(lib, "d3dx10d.lib")
	#else if
		#pragma comment(lib, "d3dx10.lib")
	#endif

	// DIRECT INPUT 8
	#pragma comment(lib, "dinput8.lib")

	// Debug build defines
	#if defined(DEBUG) || defined(_DEBUG)
		#ifndef D3D_DEBUG_INFO
		#define D3D_DEBUG_INFO
		#endif
	#endif

	// Performance Api 
	#pragma comment(lib, "pdh.lib")
	#include <pdh.h>

#endif	// End if PLATFORM _WIN32


namespace SCT
{
	// Releases the COM interface, IF the interface is set... -- Used for DirectX
	template <class T>
	void ReleaseCOM(T &COMInterface)
	{
		if(COMInterface)
		{
			COMInterface->Release();
			COMInterface	= NULL;
		}
	}
}


#endif	//\ End of Header file