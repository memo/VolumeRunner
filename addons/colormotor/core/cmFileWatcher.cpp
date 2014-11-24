/*
 *  fileWatcher.cpp
 *
 *  Created by Daniel Berio on 6/16/12.
 *  http://www.enist.org
 *  Copyright 2012. All rights reserved.
 *
 */


#include "cmFileWatcher.h"

namespace cm
{
	
/* A simple routine to return a string for a set of flags. */
static char *flagstring(int flags)
{
    static char ret[512];
    char *ors = "";
	
    ret[0]='\0'; // clear the string.
    if (flags & NOTE_DELETE) {strcat(ret,ors);strcat(ret,"NOTE_DELETE");ors="|";}
    if (flags & NOTE_WRITE) {strcat(ret,ors);strcat(ret,"NOTE_WRITE");ors="|";}
    if (flags & NOTE_EXTEND) {strcat(ret,ors);strcat(ret,"NOTE_EXTEND");ors="|";}
    if (flags & NOTE_ATTRIB) {strcat(ret,ors);strcat(ret,"NOTE_ATTRIB");ors="|";}
    if (flags & NOTE_LINK) {strcat(ret,ors);strcat(ret,"NOTE_LINK");ors="|";}
    if (flags & NOTE_RENAME) {strcat(ret,ors);strcat(ret,"NOTE_RENAME");ors="|";}
    if (flags & NOTE_REVOKE) {strcat(ret,ors);strcat(ret,"NOTE_REVOKE");ors="|";}
	
    return ret;
}

FileWatcher::FileWatcher( const std::string & path_, int msecs  )
:
fileFlags(0),
path(path_),
msecs(msecs)
{
    /* Open a kernel queue. */
    if ((kq = kqueue()) < 0) {
        fprintf(stderr, "Could not open kernel queue.  Error was %s.\n", strerror(errno));
		return;
    }
	
    /*
	 Open a file descriptor for the file/directory that you
	 want to monitor.
     */
    event_fd = open(path.c_str(), O_EVTONLY);
    if (event_fd <=0) {
        fprintf(stderr, "The file %s could not be opened for monitoring.  Error was %s.\n", path.c_str(), strerror(errno));
        return;
    }
	
	/*
	 The address in user_data will be copied into a field in the
	 event.  If you are monitoring multiple files, you could,
	 for example, pass in different data structure for each file.
	 For this example, the path string is used.
     */
    user_data = (void*)path.c_str();
	
	/* Set the timeout to wake us every half second. */
    timeout.tv_sec = 0;        // 0 seconds
    timeout.tv_nsec = msecs * 1000000;//1000000;//500000000;    // 500 milliseconds
	
    /* Set up a list of events to monitor. */
    vnode_events = NOTE_DELETE |  NOTE_WRITE | NOTE_EXTEND | NOTE_ATTRIB | NOTE_LINK | NOTE_RENAME | NOTE_REVOKE;
    EV_SET( &events_to_monitor[0], event_fd, EVFILT_VNODE, EV_ADD | EV_CLEAR, vnode_events, 0, user_data);
	
	num_files =  1;
	
	startThread();
}

FileWatcher::~FileWatcher()
{
	//stopThread();
	::close(event_fd);
}

bool FileWatcher::hasFileChanged()
{
	bool res = fileFlags != 0;
	fileFlags = 0;
	return res;
}

void FileWatcher::run()
{
	while( isThreadRunning() )
	{
		int event_count = kevent(kq, events_to_monitor, NUM_EVENT_SLOTS, event_data, num_files, &timeout);
		if ((event_count < 0) || (event_data[0].flags == EV_ERROR)) {
			/* An error occurred. */
			fprintf(stderr, "An error occurred (event count %d).  The error was %s.\n", event_count, strerror(errno));
		}
		
		if (event_count) 
		{
			printf("Event  occurred.  Filter %d, flags %d, filter flags %s\n",
				   event_data[0].filter,
				   event_data[0].flags,
				   flagstring(event_data[0].fflags));
			
			fileFlags = event_data[0].fflags;
		}
		else
		{
			fileFlags = 0;
		}
			
		/* Reset the timeout.  In case of a signal interrruption, the
		 values may change. */
		timeout.tv_sec = 0;        // 0 seconds
		timeout.tv_nsec = msecs * 1000000;//1000000;    // 200 milliseconds
		Thread::sleep(20);
	}
}


}