///////////////////////////////////////////////////////////////////////////
//	 _________  __   ____  ___  __  _______  __________  ___
//	/ ___/ __ \/ /  / __ \/ _ \/  |/  / __ \/_  __/ __ \/ _ \
// / /__/ /_/ / /__/ /_/ / , _/ /|_/ / /_/ / / / / /_/ / , _/
// \___/\____/____/\____/_/|_/_/  /_/\____/ /_/  \____/_/|_|alpha.
//
//  Daniel Berio
//	http://www.enist.org/
//
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include "CMThread.h"

#ifdef CM_WIN32
#include "platform/win32/CMWin32Thread.h"
#else
#include "platform/osx/CMOSXThread.h"
#endif


//#include <unistd.h>
/*
#ifdef CM_WIN32
#include <process.h>
#elif CM_IOS
#else
#include <pthread.h>
#include <semaphore.h>
#endif
*/

namespace cm
{

///////////////////////////////////////////////////
// Mutex definition is specific to platform

CriticalSection::CriticalSection()
{
	mutex = new Mutex();
}

CriticalSection::~CriticalSection()
{
	delete mutex;
}

void CriticalSection::lock()
{
	return mutex->lock();
}

bool CriticalSection::tryLock()
{
	return mutex->tryLock();
}

void CriticalSection::unlock()
{
	mutex->unlock();
}

///////////////////////////////////////////////////


Thread::Thread()
:
threadRunning(false)
{ 
} 

Thread::~Thread()
{ 
	stopThread(); 
} 

bool Thread::isThreadRunning()
{ 
   //should be thread safe - no writing of vars here 
   return threadRunning; 
} 

/*
void Thread::startThread(bool _blocking)
{ 
	if( threadRunning )
	{ 
		printf("CMThread: thread already running\n"); 
		return; 
	} 

	//have to put this here because the thread can be running 
	//before the call to create it returns 
	threadRunning   = true; 

	#ifdef CM_WIN32 
	thread = (HANDLE)_beginthreadex(NULL, 0, this->threadIt,  (void *)this, 0, NULL); 
	#elif CM_IOS
	thread = 0;
	#else 
	pthread_create(&thread, NULL, threadIt, (void *)this); 
	#endif 

	blocking = _blocking; 
} 
*/

//------------------------------------------------- 
//returns false if it can't lock 
bool Thread::lockThread()
{ 
	if(blocking)
	{
		cs.lock();
		return true; 
	} else {
		return cs.tryLock();
	}
	
} 

//------------------------------------------------- 
bool Thread::unlockThread()
{ 
	cs.unlock();
	return true; 
} 

#ifdef WIN32
#else
void Thread::sleep(int milliseconds)
{
	usleep(milliseconds*1000);			
}
#endif


/*
void Thread::stopThread()
{ 
	if(threadRunning)
	{ 
		#ifdef CM_WIN32 
		CloseHandle(thread); 
		#elif CM_IOS
		thread = 0;
		#else
		pthread_detach(thread); 
		#endif 
		printf("CMThread: thread stopped\n"); 
		threadRunning = false; 
	} else { 
		printf("CMThread: thread already stopped\n"); 
	} 
	#ifdef CM_WIN32 
		Sleep(5000);
	#else
		usleep(500000); // some time while thread is stopping ...		
	#endif
	
}

*/

}