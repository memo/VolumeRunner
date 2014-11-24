#pragma once

#define _WIN32_WINNT  0x0400 // need for using TryEnterCriticalSection 

#include <windows.h>
#include <process.h>

#define THREAD ((HANDLE)thread)
namespace cm
{

///////////////////////////////////////////////////

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

	thread = 0;//(HANDLE)_beginthreadex(NULL, 0, this->threadIt,  (void *)this, 0, NULL); 

	blocking = _blocking; 
} 

///////////////////////////////////////////////////

void Thread::stopThread()
{ 
	if(threadRunning)
	{ 
		CloseHandle(thread); 
		printf("CMThread: thread stopped\n"); 
		threadRunning = false; 
	} else { 
		printf("CMThread: thread already stopped\n"); 
	} 
	
	Sleep(5000);	
}

///////////////////////////////////////////////////

class Mutex
{
private:
	CRITICAL_SECTION  mutex;  	//same as a mutex	
public:
	Mutex()
	{
		InitializeCriticalSection(&mutex); 		
	}
	
	~Mutex()
	{
		DeleteCriticalSection(&mutex);		
	}
	
	bool tryEnter()
	{
		return false;
		/*
		if(!TryEnterCriticalSection(&mutex))
		{ 
			printf("CriticalSection: mutext is busy \n"); 
			return false; 
		} */
		return true;
	}
	
	void enter()
	{
		EnterCriticalSection(&mutex); 	
	}
	
	void exit()
	{
		LeaveCriticalSection(&mutex); 
	}
};



}







