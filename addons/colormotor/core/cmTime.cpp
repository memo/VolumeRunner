/*
 *  CMTime.cpp
 *  vboPerformanceTest
 *
 *  Created by ensta on 5/7/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include "CM.h"
#include "CMTime.h"

#ifdef CM_WINDOWS
#include <windows.h>
#endif



static double curTime = 0.0;
static double frameTime = 0.0;

static double fpsBase = 0.0f;

static int frames = 0;
static double fps = 60.0;
static double targetFps = 60.0;
static double fpsRatio = 1.0;

namespace cm
{

void updateTime( double curTimeMsecs )
{
	static bool firstTime = true;
	
	if (curTimeMsecs == -1.0)
		curTimeMsecs = getTickCount();
	
	if( firstTime )
	{
		curTime = curTimeMsecs;
		fpsBase = curTime;
		firstTime = false;
	}
	
	frameTime = curTimeMsecs-curTime;
	curTime = curTimeMsecs;
	
	// compute fps
	frames++;
	if (curTime - fpsBase > 1000) 
	{
		fps = frames*1000.0/(curTime-fpsBase);
		fpsRatio = targetFps / fps;
		fpsBase = curTime;		
		frames = 0;
	}
}

double getFPS()
{
	return fps;
}

void setTargetFps(double targetFps_)
{
	targetFps = targetFps_;
}

double getFpsRatio()
{
	return fpsRatio;
}


double getCurrentTime()
{
	return curTime;
}

double getFrameTime()
{
	return frameTime;
}

double getTickCount()
{
	#ifdef CM_WINDOWS
	return (double) GetTickCount();
	#else
	return CFAbsoluteTimeGetCurrent()*1000;
	#endif
	return 0.0;
}





///////////////////////////////////////////////////////////////////////////////
// constructor
///////////////////////////////////////////////////////////////////////////////
PreciseTimer::PreciseTimer()
{
#ifdef WIN32
    QueryPerformanceFrequency(&frequency);
    startCount.QuadPart = 0;
    endCount.QuadPart = 0;
#else
    startCount.tv_sec = startCount.tv_usec = 0;
    endCount.tv_sec = endCount.tv_usec = 0;
#endif

    stopped = 0;
    startTimeInMicroSec = 0;
	lastTimeInMicroSec = 0;
    endTimeInMicroSec = 0;
}



///////////////////////////////////////////////////////////////////////////////
// distructor
///////////////////////////////////////////////////////////////////////////////
PreciseTimer::~PreciseTimer()
{
}



///////////////////////////////////////////////////////////////////////////////
// start timer.
// startCount will be set at this point.
///////////////////////////////////////////////////////////////////////////////
void PreciseTimer::start()
{
    stopped = 0; // reset stop flag
#ifdef WIN32
    QueryPerformanceCounter(&startCount);
#else
    gettimeofday(&startCount, NULL);
#endif
}



///////////////////////////////////////////////////////////////////////////////
// stop the timer.
// endCount will be set at this point.
///////////////////////////////////////////////////////////////////////////////
void PreciseTimer::stop()
{
    stopped = 1; // set timer stopped flag

#ifdef WIN32
    QueryPerformanceCounter(&endCount);
#else
    gettimeofday(&endCount, NULL);
#endif
}



///////////////////////////////////////////////////////////////////////////////
// compute elapsed time in micro-second resolution.
// other getElapsedTime will call this first, then convert to correspond resolution.
///////////////////////////////////////////////////////////////////////////////
double PreciseTimer::getElapsedTimeInMicroSec()
{
#ifdef WIN32
    if(!stopped)
        QueryPerformanceCounter(&endCount);

    startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
    endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);
#else
    if(!stopped)
        gettimeofday(&endCount, NULL);

    startTimeInMicroSec = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
    endTimeInMicroSec = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;
#endif

    return endTimeInMicroSec - startTimeInMicroSec;
}


double PreciseTimer::getFrameTimeInMicroSec()
{
	double old = lastTimeInMicroSec;
#ifdef WIN32
	if(!stopped)
		QueryPerformanceCounter(&curCount);

    lastTimeInMicroSec = curCount.QuadPart * (1000000.0 / curCount.QuadPart);

#else

	gettimeofday(&curCount, NULL);

	//lastTimeInMicroSec = (double)curCount.tv_sec + (double)(curCount.tv_usec / 1000000.0);
	lastTimeInMicroSec = (double)(curCount.tv_sec * 1000000 + (curCount.tv_usec));

#endif

    return lastTimeInMicroSec - old;
}

///////////////////////////////////////////////////////////////////////////////
// divide elapsedTimeInMicroSec by 1000
///////////////////////////////////////////////////////////////////////////////
double PreciseTimer::getElapsedTimeInMilliSec()
{
    return this->getElapsedTimeInMicroSec() * 0.001;
}



///////////////////////////////////////////////////////////////////////////////
// divide elapsedTimeInMicroSec by 1000000
///////////////////////////////////////////////////////////////////////////////
double PreciseTimer::getElapsedTimeInSec()
{
    return this->getElapsedTimeInMicroSec() * 0.000001;
}



///////////////////////////////////////////////////////////////////////////////
// same as getElapsedTimeInSec()
///////////////////////////////////////////////////////////////////////////////
double PreciseTimer::getElapsedTime()
{
    return this->getElapsedTimeInSec();
}


}