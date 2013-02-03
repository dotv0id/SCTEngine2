// -------------------- +
//		Includes
// -------------------- +

#include <Core\SCTTime.h>
#include <SCTPlatform.h>


namespace SCT
{
namespace Core
{

SCTTime::SCTTime() 
: mSecondsPerCount(0), mElapsedTime(-1.0f), 
  mBaseTime(0), mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
{
}

SCTTime::~SCTTime()
{
}

SCTReturn SCTTime::Initialize()
{
	SUInt64 countsPerSec;

	// Check to see if this system supports high performance timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	if(countsPerSec == 0)
		return FAIL;

	// Find out how many times the frequency counter ticks every millisecond.
	mSecondsPerCount = (double)(1.0 / countsPerSec);

	return OK;
}

SCTReturn SCTTime::Shutdown()
{
	return OK;
}

void SCTTime::Start()
{
	SUInt64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	/* Accumulate the time elapsed between stop and start pairs.
	 *
	 *                     |<-------d------->|
	 * ----*---------------*-----------------*------------> time
	 *  mBaseTime       mStopTime        startTime     
	 */

	if(mStopped)
	{
		mPausedTime += (startTime - mStopTime);	

		mPrevTime = startTime;
		mStopTime = 0;
		mStopped  = false;

		#if defined(DEBUG) || defined(_DEBUG)
			std::cout<<"Game Resuming\n";
		#endif
	}
}

void SCTTime::Reset()
{
	SUInt64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped  = false;
}

void SCTTime::Stop()
{
	if(!mStopped)
	{
		SUInt64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mStopTime = currTime;
		mStopped  = true;

		#if defined(DEBUG) || defined(_DEBUG)
			std::cout<<"Game Paused\n";
		#endif
	}
}

void SCTTime::Tick()
{
	if(mStopped)
	{
		mElapsedTime = 0.0;
		return;
	}

	SUInt64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;

	// Time difference between this frame and the previous.
	mElapsedTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

	// Prepare for next frame.
	mPrevTime = mCurrTime;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if(mElapsedTime < 0.0)
		mElapsedTime = 0.0f;
}

float SCTTime::GetGameTime()
{
	/* If we are stopped, do not count the time that has passed since we stopped.
	 *
	 * ----*---------------*------------------------------*------> time
	 *  mBaseTime       mStopTime                      mCurrTime
	 */

	/* The distance mCurrTime - mBaseTime includes paused time,
	 * which we do not want to count.  To correct this, we can subtract 
	 * the paused time from mCurrTime:  
	 *
	 *  (mCurrTime - mPausedTime) - mBaseTime 
	 *
	 *                     |<-------d------->|
	 * ----*---------------*-----------------*------------*------> time
	 *  mBaseTime       mStopTime        startTime     mCurrTime
	 */

	if(mStopped)
		return (float)( (mStopTime - mBaseTime) * mSecondsPerCount );
	else
		return (float)( ( (mCurrTime - mPausedTime) - mBaseTime) * mSecondsPerCount );
}

}	// End of Core	namespace
}	// End of SCT	namespace