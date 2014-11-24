/*
 *  cmMemoryMap.cpp
 *  pointTest
 *
 *  Created by Daniel Berio on 8/30/09.
 *  Copyright 2009 http://www.enist.org. All rights reserved.
 *
 */

#include "cmMemoryMap.h"
#include "cmUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>

namespace cm
{

/////////////////////////////////////////////////////////////

// FIXME hacky shit.
bool MemoryMap::open( const char * file, unsigned int flags  )
{
	_flags = flags;
	
	if( flags == MMAP_READONLY )
	{
		_handle = ::open(file, flags);
	}
	else
	{
		if( flags == MMAP_READWRITE )
		{
			_handle = ::open (file, O_RDWR | O_CREAT | O_TRUNC, 0);
		}
		else
			_handle = ::open(file, flags);
	}
	
	if(_handle < 0)
		return false;
		
	return true;
}

void	MemoryMap::close()
{
	if(_map)
		unmap();
	if(_handle)
		::close(_handle);
	_handle = 0;
}

/////////////////////////////////////////////////////////////


bool MemoryMap::map( int length )
{
	// FIXME hacky shit.
	if(_map)
		unmap();
	
	_length = length;
	
	// check if we are writing
	if(_flags == MMAP_READWRITE)
	{
		// write end char to end of file
		char endchar='\0'; 

		lseek(_handle, length-1, SEEK_SET); 
		write(_handle, &endchar, 1);
		
		if((_map = (char*)mmap(0, length, PROT_WRITE, MAP_SHARED, _handle, 0)) == (char *)-1) 
		{ 
			debugPrint( "Could not map output file\n"); 
			return false;
		} 

		return true;
	}
	
	
	_map = (char *)mmap(0, length, PROT_READ, MAP_SHARED, _handle, 0);
	if (_map == MAP_FAILED)
			return false;
	_length = length;
	return true;
}

/////////////////////////////////////////////////////////////

void MemoryMap::unmap()
{
	munmap((char *)_map, getLength());
	_length = 0;
	_map = 0;
}

/////////////////////////////////////////////////////////////

size_t	MemoryMap::getFileLength()
{
	struct stat statbuf;
	if (fstat(_handle, &statbuf) == -1)
		return -1;
	return statbuf.st_size;
}			
			
		

}