/**
----------------------------------------------------------------------------
	@File:			SCTTime.h
   
	System:         SCTGame Engine 
	Component Name:	SCT High Definition Timer
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


#ifndef SCT_TIME_H
#define SCT_TIME_H


#include <SCT.h>
#include <SCTSingleton.h>


namespace SCT
{
	namespace Core
	{
		//	Class name:	 SCTHdTimer
		//	Description: provides a High definition timer
		//	! TODO;		 platform independant 
		class SCTTime : public Singleton<SCTTime>
		{
		public:
			SCTTime();
			~SCTTime();

			SCTReturn			Initialize();
			SCTReturn			Shutdown();

			void				Start();		// Call when unpaused.
			void				Reset();		// Call before message loop.
			void				Stop();			// Call when paused.
			void				Tick();			// Call every frame.

			inline bool			IsPaused() { return mStopped; }

			// Get, Set functions
			inline float		GetElapsedTime() { return (float)mElapsedTime; }	// Get Elapsed time in seconds
			float				GetGameTime();										// in seconds
			
		private:
			double				mSecondsPerCount,
								mElapsedTime;

			SUInt64				mBaseTime,
								mPausedTime,
								mStopTime,
								mPrevTime,
								mCurrTime;

			bool				mStopped;				// Should the timer count?

		};	// End of SCTHdTimer Class definition

	}	// End of Core	namespace

	// Singleton
	template<> Core::SCTTime* Singleton<Core::SCTTime>::ms_Singleton = 0;

}		// End of SCT	namespace

#endif // SCTHdTimer