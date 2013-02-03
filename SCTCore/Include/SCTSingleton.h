/**
----------------------------------------------------------------------------
	@File:			SCTSingleton.h
   
	System:         SCTGame Engine 
	Component Name:	SCT singleton design pattern - taken from Ogre3D & GPUGems #1
	Status:         Version 1.0 Release 1 

	Language:		C++
   
	License:		GNU Public License	[*!]
	Licensed		Material - Property of Ogre3D (and/or GPU Gems #1)
   
	Author:			Anastasios 'v0id' Giannakopoulos
	E-Mail:			dotvoidd@gmail.com
   
	Description:	Header file for Project X
					This file contains the defined types for Project X
					This is sometimes called the "Abstract" and may be
					followed by a section called "Notes".

----------------------------------------------------------------------------
*/

#ifndef SCT_SINGLETON_H
#define SCT_SINGLETON_H

#include <cassert>

namespace SCT
{
	// Singleton class - taken from Ogre3D
	// ...
	template <typename T> class Singleton
	{
	private:
		// brief Explicit private copy constructor. This is a forbidden operation.
		Singleton(const Singleton<T> &);

		// brief Private operator= . This is a forbidden operation.
		Singleton& operator = (const Singleton<T> &);
    
	protected:
		static T* ms_Singleton;

	public:
		Singleton(void)
		{
			assert(!ms_Singleton);

			#if defined( _MSC_VER ) && _MSC_VER < 1200
	           SINT32 offset = (SINT32)(T*)1 - (SINT32)(Singleton <T>*)(T*)1;
	           ms_Singleton = (T*)((SINT32)this + offset);
			#else
				ms_Singleton = static_cast< T* >(this);
			#endif
		}

		~Singleton(void)
		{
			assert(ms_Singleton);
			ms_Singleton = 0;
		}

		static T& getSingleton(void)
		{
			assert(ms_Singleton); 
			return (*ms_Singleton);
		}

		static T* getSingletonPtr(void)
		{
			return ms_Singleton;
		}
	};	// End of Singleton class
}	// End of SCT namespace

#endif // SCTSINGLETON_H