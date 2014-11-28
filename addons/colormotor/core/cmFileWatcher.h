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

#pragma once

#include "cmCore.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>

namespace cm
{
	
class FileWatcher : public Thread
{
public:
	FileWatcher( const std::string & path, int msecs = 200 );
	~FileWatcher();
	
	bool hasFileChanged();
	
	void run();
	
	
private:
	enum
	{
		NUM_EVENT_SLOTS = 1,
		NUM_EVENT_FDS  = 1
	};
	
	int msecs;
	unsigned int fileFlags;
	std::string path;
	int kq;
    int event_fd;
    struct kevent events_to_monitor[1];
    struct kevent event_data[1];
    void *user_data;
    struct timespec timeout;
    unsigned int vnode_events;
	int num_files;
};

}