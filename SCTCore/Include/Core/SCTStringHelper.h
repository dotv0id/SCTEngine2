/**
----------------------------------------------------------------------------
	@File:			SCTStringHelper.h
   
	System:         SCTGame Engine 
	Component Name:	SCT string helping functions
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


#ifndef SCT_STRING_HELPER_H
#define SCT_STRING_HELPER_H


#include <SCT.h>
#include <SCTSingleton.h>


namespace SCT
{
	namespace Core
	{
		// Class name:	SCTStringHelper
		// Description: provides string manipulation functions
		// ...
		class SCTStringHelper : public Singleton<SCTStringHelper>
		{
		public:
			SCTStringHelper();
			~SCTStringHelper();

			SCTReturn					Initialize();
			SCTReturn					Shutdown();

			// split string to specific character. returns std vector
			std::vector<std::string>	tokenizeString(const std::string & str, const std::string & delims="\n");

			// ascii string to unicode string
			std::wstring				stringToWString(const std::string& s);
			std::string					wStringToString(const std::wstring& ws);

			// removes whitespaces from string	
			std::string					removeWhitespaces(const std::string &inString);
	
			template <class TNUM>
			std::wstring				numberToWstring(TNUM numToConvert)
			{
				std::wstringstream lConvert;

				TNUM lTmp = numToConvert;
				lConvert << lTmp;

				return lConvert.str().c_str();
			}

			template <class TNUM>
			std::string				numberToString(TNUM numToConvert)
			{
				std::stringstream lConvert;

				TNUM lTmp = numToConvert;
				lConvert << lTmp;

				return lConvert.str().c_str();
			}

			template <class TNUM>
			bool					stringToNumber(TNUM& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
			{
				std::stringstream iss(s);
				return !(iss >> f >> t).fail();
			}

		private:

		};	// End of SCTStringHelper Class definition

	}	// End of Core	namespace

	// Singlenton
	template<> Core::SCTStringHelper* Singleton<Core::SCTStringHelper>::ms_Singleton = 0;

}		// End of SCT	namespace

#endif