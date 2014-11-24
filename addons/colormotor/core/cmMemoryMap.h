/*
 *  cmMemoryMap.h
 *  pointTest
 *
 *  Created by Daniel Berio on 8/30/09.
 *  Copyright 2009 http://www.enist.org. All rights reserved.
 *
 */

#pragma once 

#include "CMCoreIncludes.h"

namespace cm
{
	// FIXME hacky shit.
	enum
	{
		MMAP_READONLY = 0,
		MMAP_WRITEONLY = 1,
		MMAP_READWRITE = 2
	};

	class MemoryMap
	{
		public:
			MemoryMap()
			{
				_handle = 0;
				_length = 0;
				_map = 0;
			}
			
			~MemoryMap()
			{
				_handle = 0;
				_length = 0;
				
				if(_map)
					unmap();
				if(_handle)
					close();
			}
			
			/// open file
			bool open( const char * file, unsigned int flags = MMAP_READONLY );
			/// close file
			void close();
			
			/// map length bytes to virtual memory
			bool map( int length );
			///
			void unmap();
			///
			size_t	getFileLength();
			int		getLength() const { return _length; }
			
			operator void * () { return (void*)_map; }
			operator char * () { return _map; }
			
			void * get() { return (void*)_map; }
			
		protected:
			int		_handle;
			char *	_map;
			int		_length;
			
			unsigned int _flags;
	};
}