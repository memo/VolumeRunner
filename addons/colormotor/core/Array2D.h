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

#include <vector>

/// 2D Array container,
/// Highly unoptimized with insertion and resizing but handy....

namespace cm
{

template <class OBJTYPE> class Array2D
{
private:
	void _init()
	{
		buf.clear();
		_width = 0;
		_height = 0;
	}
	
	inline void setBuf( int x, int y, const OBJTYPE & obj )
	{
		buf[y*_width+x] = obj;
	}
	
	// makes it easier to use naaasty copy and copy_backward
	void move( int dest, int from, int n )
	{
		if( dest < from )
		{
			// copy
			std::copy(	buf.begin() + from,
						buf.begin() + from + n,
						buf.begin() + dest );
		}
		else if( dest > from )
		{
			// copy back
			std::copy_backward(	buf.begin() + from,
							    buf.begin() + from + n,
								buf.begin() + dest + n );
		}
	}
	
	int _width;
	int _height;
	
	//OBJTYPE * buf;
	std::vector <OBJTYPE> buf;
	
public:
	Array2D()
	{
		_init();
	}
	
	Array2D( const Array2D<OBJTYPE> & ar )
	{
		clear();
		_width = ar.width;
		_height = ar.height;
		buf = ar.buf;
	}
	
	Array2D( int w, int h )
	{
		_init();
		resize(w,h);
	}
	
	~Array2D()
	{
		clear();
	}
	
	void clear()
	{
		buf.clear();
		_width = 0;
		_height = 0;
		_init();
	}
	
	
	void resize( int w, int h ) 
	{
		if( w < 0 )
			w = 0;
		if( h < 0 )
			h = 0;
			
		std::vector<OBJTYPE> oldbuf = buf;
		printf("oldbuf %d   buf %d\n",oldbuf.size(),buf.size());
		
		int oldw = _width;
		int oldh = _height;
		_width = w;
		_height = h;

		int oldn = oldw*oldh;
		int n = w*h;
		
		if( n )
		{
			buf.clear();
			buf.resize(_width*_height);
			
			// copy in old data
			for( int y = 0; y < oldh; y++ )
				for( int x = 0; x < oldw; x++ )
				{
					buf[y*_width+x] = oldbuf[y*oldw+x];
				}
		}
		
		oldbuf.clear();
	}
	
	const OBJTYPE & operator() ( int x, int y ) const { return buf[_width*y+x]; }
	OBJTYPE & operator() ( int x, int y )  { return buf[_width*y+x]; }
	OBJTYPE *get() { return &buf[0]; }
	const OBJTYPE *get() const { return &buf[0]; }

	/// Access consectuive elements on row or column
	OBJTYPE & access( int rowOrColumn, int rowOrColumnIndex, int index )
	{
		if( rowOrColumn ) // col
		{
			return buf[_width*index+rowOrColumnIndex];
		}
		else
		{
			return buf[_width*rowOrColumnIndex+index];
		}
	}
	
	const OBJTYPE & access( int rowOrColumn, int rowOrColumnIndex, int index ) const
	{
		if( rowOrColumn ) // col
		{
			return buf[_width*index+rowOrColumnIndex];
		}
		else
		{
			return buf[_width*rowOrColumnIndex+index];
		}
	}

	
	void addRows( int num )
	{
		resize( _width, _height+num );
	}
	
	void addColumns( int num )
	{
		resize( _width+num, _height );
	}
	
	void removeRows( int num )
	{
		resize( _width, _height-num );
	}
	
	void removeColumns( int num )
	{
		resize( _width-num,_height );
	}
	
	void insertRows( int at, int num )
	{
		if( at >= _height )
			return addRows( num );
			
		int n = _height-at;
		
		resize( _width, _height+num );
		
		int start = _width*at;
		
		move( _width*(at+num), _width*at, _width*n );
	}
	
	void insertColumns( int at, int num )
	{
		if( at >= _width )
			return addColumns( num );
			
		int n = _width-at;
		
		resize( _width+num, _height );
	
		for( int y = 0; y < _height; y++ )
		{
			move( _width*y+(at+num), _width*y+at, n );
		}
	}
	
	
	void removeRows( int at, int num )
	{
		if( at >= _height )
			return removeRows( num );
			
		int n = _height-at;
		
		// move
		move( _width*at, _width*(at+num), _width*n );
		
		// realloc
		removeRows(num);
	}
	
	
	void removeColumns( int at, int num )
	{
		if( at >= _width )
			return removeColumns( num );
			
		int n = _width-at;
		
		for( int y = 0; y < _height; y++ )
		{
			// move stuff..
			move(_width*y+at, _width*y+(at+num), n );
		}
		
		// realloc
		removeColumns(num);
	}


	int width() const { return _width; }
	int height() const { return _height; }
	int size() const { return _width*_height; }
	
	
};


}