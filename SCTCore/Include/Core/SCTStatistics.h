/**
----------------------------------------------------------------------------
	@File:			SCTStatistics.h
   
	System:         SCTGame Engine 
	Component Name:	SCT Statistics
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


#ifndef SCT_STATISTICS_H
#define SCT_STATISTICS_H


#include <SCT.h>
#include <SCTSingleton.h>
#include <SCTPlatform.h>


namespace SCT
{
	namespace Core
	{
		// Class name:	 SCTStatistics
		// Description: provides SCTEngine statistics (framerate counter, cpu usage, etc)
		// ...
		class SCTStatistics : public Singleton<SCTStatistics>
		{
		public:
			SCTStatistics();
			~SCTStatistics();

			SCTReturn				Initialize();
			SCTReturn				Update();
			SCTReturn				Shutdown();

			// Get, Set Functions
			inline void				SetFpsCounterInterval(SUInt32 CounterInterval) { mCounterInterval = CounterInterval; }
			void					GetFrameStats(float &Fps, float &MsPFrame);
			void					GetCpuPercentage(SUInt32 &cpuPercentage);

			inline void				EnableFpsStats(bool Mode) { mFpsStats	= Mode; }
			inline void				EnableCPUStats(bool Mode) { mCpuStats	= Mode; }

		private:
			// --- Private Variables
			// Frame rate counter
			float					mFps;
			float					mMsPerFrame;
			SUInt64					mFrameCnt;
			double					t_base;

			SUInt32					mCounterInterval;

			bool					mFpsStats,
									mCpuStats;

			// CPU stats
			bool					mCanReadCpu;
			HQUERY					mQueryHandle;
			HCOUNTER				mCounterHandle;

			SUInt64					mLastSampleTime,
									mCpuUsage;

			// --- Private Functions
			void					CalculateFpsStats();
			void					CalculateCpuStats();
		};
	}	// End of namespace Core

	// Singlenton
	template<> Core::SCTStatistics* Singleton<Core::SCTStatistics>::ms_Singleton = 0;

}	// End of namespace SCT

#endif