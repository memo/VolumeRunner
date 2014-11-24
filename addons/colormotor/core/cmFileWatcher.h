/*
 *  fileWatcher.h
 *
 *  Created by Daniel Berio on 6/16/12.
 *  http://www.enist.org
 *  Copyright 2012. All rights reserved.
 *
 */

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