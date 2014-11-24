#pragma once

#include "CMCoreIncludes.h"

namespace cm
{
	class Mutex;
	
	class CriticalSection
	{
	public:
		CriticalSection();
		virtual ~CriticalSection();
		
		void lock();
		void unlock();
		bool tryLock();
				
		Mutex* mutex;		
	};
	
	class ScopedLock
	{
	public:
		inline explicit ScopedLock (CriticalSection& lock) throw() 
		: lock (lock) 
		{ 
			lock.lock(); 
		}
		inline ~ScopedLock() throw() 
		{ 
			lock.unlock(); 
		}
	private:
		CriticalSection& lock;
	};	

	
	class Thread
	{
	public:
		Thread();
		virtual ~Thread();
		bool isThreadRunning();
		void startThread(bool _blocking = true);
		bool lockThread();
		bool unlockThread();
		void stopThread();
		
		CriticalSection & getCriticalSection() { return cs; }
		
		static void sleep(int milliseconds);
		
		//you need to overide this with the function you want to thread
		virtual void run()
		{
			printf("CMThread: overide run with your own\n");
		}
	protected:
		CriticalSection cs;
		
		void * thread;
		bool threadRunning;
		bool blocking;
	};
}
