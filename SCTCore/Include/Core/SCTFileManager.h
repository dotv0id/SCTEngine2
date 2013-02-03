/**
----------------------------------------------------------------------------
	@File:			SCTFileManager.h
   
	System:         SCTGame Engine 
	Component Name:	SCT File Manager
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


#ifndef SCT_FILE_MANAGER_H
#define SCT_FILE_MANAGER_H


#include <SCT.h>
#include <SCTSingleton.h>


namespace SCT
{
	namespace Core
	{
		// Class name:	SCTFileManager
		// Description: Provides file io and maninpulation functions
		// ...
		class SCTFileManager : public Singleton <SCTFileManager>
		{
		public:
			SCTFileManager();
			~SCTFileManager();
		
			SCTReturn					Initialize();
			SCTReturn					Shutdown();

			bool						ReadConfigFile(const std::string &cnfgFilename, std::string &outConfigStream);
			bool						WriteConfigFile(const std::string &cnfgFilename, const std::string &inpConfigStream);
			
			SCTFontType*				LoadFontData(const char *filename);

		private:

		};	// End of SCTFileManager Class definition

	}	// End of Core	namespace

	// Singlenton
	template<> Core::SCTFileManager* Singleton<Core::SCTFileManager>::ms_Singleton = 0;

}		// End of SCT	namespace

#endif