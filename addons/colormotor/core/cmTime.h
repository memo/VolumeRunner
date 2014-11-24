/*
 *  CMTime.h
 *  vboPerformanceTest
 *
 *  Created by ensta on 5/7/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include <vector>
#ifdef WIN32   // Windows system specific
#include <windows.h>
#else          // Unix based system specific
#include <sys/time.h>
#endif

namespace cm
{

/// All timing is in milliseconds

void updateTime( double curTimeMsecs = -1.0);
double getFPS();
void setTargetFps(double targetFps_);
double getFpsRatio();
double getCurrentTime();
double getFrameTime();
double getTickCount();

inline double timeCorrect( double val, double msecs, double fps )
{
	return msecs*0.001*fps*val;
} 


class Timer
{
public:
	Timer()
	{
	}

	virtual ~Timer()
	{
		intervals.clear();
	}

	struct Interval
	{
		double durationMsecs;
		double curMsecs;
		double t; // 0 1 cmtio of time interval
		int	  timerId;
		bool	loop;
	};

	void addInterval( int id_, double durationMsecs, bool loop = true )
	{
		Interval i;
		i.timerId = id_;
		i.durationMsecs = durationMsecs;
		i.curMsecs = 0;
		i.t = 0.0;
		i.loop = loop;
		intervals.push_back(i);
	}

	void	removeInterval( int id_ )
	{
		for( int i = 0; i < intervals.size(); i++ )
			if(intervals[i].timerId == id_)
			{
				intervals.erase(intervals.begin()+i);
				return;
			}
	}

	 
	void	updateTimer( double msecs )
	{
		if(msecs>200)
			msecs = 200;
			
		for( int i = 0; i < intervals.size(); i++ )
		{
			Interval & iv = intervals[i];
			iv.curMsecs+=msecs;
			iv.t = iv.curMsecs/iv.durationMsecs;
			
			if(iv.curMsecs>=iv.durationMsecs)
			{
				iv.t = 1.0;
				
				while(iv.curMsecs>=iv.durationMsecs)
				{
					iv.curMsecs-=iv.durationMsecs;
				}
				
				onInterval(iv);
				
				if( iv.loop == false )
				{
					removeInterval( iv.timerId );
				}
			}
		}
	}

	virtual void onInterval( const Interval & interval ) {}
	int getNumIntervals() const { return intervals.size(); }
	const Interval & getIntervalAt( int index ) const { return intervals[index]; }
	Interval & getIntervalAt( int index ) { return intervals[index]; }
	
	private:
		std::vector <Interval> intervals;
};

//////////////////////////////////////////////////////////////////////////////
// Timer.h
// =======
// High Resolution Timer.
// This timer is able to measure the elapsed time with 1 micro-second accuracy
// in both Windows, Linux and Unix system 
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2003-01-13
// UPDATED: 2006-01-13
//
// Copyright (c) 2003 Song Ho Ahn
//////////////////////////////////////////////////////////////////////////////


class PreciseTimer
{
public:
    PreciseTimer();                                    // default constructor
    ~PreciseTimer();                                   // default destructor

    void   start();                             // start timer
    void   stop();                              // stop the timer
    double getElapsedTime();                    // get elapsed time in second
    double getElapsedTimeInSec();               // get elapsed time in second (same as getElapsedTime)
    double getElapsedTimeInMilliSec();          // get elapsed time in milli-second
    double getElapsedTimeInMicroSec();          // get elapsed time in micro-second

	double getFrameTimeInMicroSec();
protected:


private:
    double startTimeInMicroSec;                 // starting time in micro-second
    double endTimeInMicroSec;                   // ending time in micro-second
	double lastTimeInMicroSec;
    int    stopped;                             // stop flag 
#ifdef WIN32
    LARGE_INTEGER frequency;                    // ticks per second
    LARGE_INTEGER startCount;                   //
    LARGE_INTEGER endCount;                     //
	LARGE_INTEGER curCount;
#else
    timeval startCount;                         //
    timeval endCount;                           //
	 timeval curCount;                           //
#endif
};

}