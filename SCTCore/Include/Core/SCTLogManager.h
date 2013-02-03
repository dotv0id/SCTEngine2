/**
----------------------------------------------------------------------------
	@File:			SCTLogManager.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Log Manager
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


#ifndef SCT_LOG_MANAGER_H
#define SCT_LOG_MANAGER_H


#include <SCT.h>
#include <SCTSingleton.h>
#include <SCTPlatform.h>


namespace SCT
{
	namespace Core
	{
		//	Class name:	 SCTErrorHandling
		//	Description: provides error handling and debugging functions
		//
		class SCTLogManager : public Singleton <SCTLogManager>
		{
		public:
			SCTLogManager();
			~SCTLogManager();
	
			SCTReturn						Initialize();
			SCTReturn						Shutdown();

			// Display error. Decide wheather to display it with msgbox or in game
			void							displayError(SCTReturn  lError);
			void							displayError(HRESULT hr);

			void							showDebugConsole();
			void							hideDebugConsole();

			// Prints a string to the debug console
			void							PrintDebug(const char* format, ...);

			// Prints the SCTConfigurations struct to the debug console
			void							coutSCTConfig(const SCTConfigRender &inConf);

			void							PrintShaderMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename);

		private:

		};	// End of SCTErrorHandling Class definition

	}	// End of Core	namespace

	// Singlenton
	template<> Core::SCTLogManager* Singleton<Core::SCTLogManager>::ms_Singleton = 0;
	
}		// End of SCT	namespace

#endif