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

#include <stdio.h>
#include <pthread.h>

namespace cm
{

	
#define THREAD ((pthread_t)thread)

static void * threadIt(void * objPtr)
{
	Thread* me	= (Thread*)objPtr;
	me->run();
	me->stopThread();
	return 0;
}

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

	pthread_t t = (pthread_t)thread;
	pthread_create(&t, NULL, threadIt, (void *)this); 

	blocking = _blocking; 
 } 
 



void Thread::stopThread()
{ 
	 if(threadRunning)
	 { 
		 pthread_detach(THREAD); 
		
		 printf("CMThread: thread stopped\n"); 
		 threadRunning = false; 
	 } 
	 else 
	 { 
		 printf("CMThread: thread already stopped\n"); 
	 } 
	 usleep(500000); // some time while thread is stopping ...		
}
 
 


class Mutex
{
private:
	pthread_mutex_t  mutex;
public:
	Mutex()
	{
		pthread_mutex_init(&mutex, NULL); 
	}
	
	~Mutex()
	{
		pthread_mutex_destroy(&mutex); 
	}
	
	bool tryLock()
	{
		int value = pthread_mutex_trylock(&mutex); 
		if(value != 0)
		{
			printf("CMThread: mutext is busy - already locked\n"); 
			return false; 
		} 
		return true;
	}
	
	void lock()
	{
		pthread_mutex_lock(&mutex); 
	}
	
	void unlock()
	{
		pthread_mutex_unlock(&mutex); 
	}
};







}