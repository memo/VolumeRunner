/*
 *  cmBuffer.h
 *
 *  Created by ensta on 10/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include "cmUtils.h"

namespace cm
{
	class Buffer
	{
	public:
		Buffer()
		{
			_buf = 0;
			release();
		}

		~Buffer()
		{
			release();
		}

		void	release()
		{
			if(_buf &&  !_bExternal)
				delete _buf;
			_bExternal = false;
			_buf = 0;
			_size = 0;
			_cur = 0;
		}

		void	set( void * data, size_t size = 0 )
		{
			if( size == 0 )
				size = _size;
			memcpy(_buf,data,size);
		}
		
		bool	allocate( size_t numBytes ) 
		{
			release();
			_size = numBytes;
			_buf = new char[_size];
			if(!_buf)
				return false;
			memset(_buf,0,_size);
			_cur = 0;
			_bExternal = false;
			return true;
		}

		void	fromBuffer( void * buf, size_t size, bool copy )
		{
			release();
			_buf = (char*)buf;
			_cur = 0;
			_size = size;
			_bExternal = true;
			if( copy )
				set(buf,size);
		}

		bool	fromFile( const char * path, bool addStringTermination  = true ) 
		{
			FILE* file = fopen(path, "rb");
			if(!file)
			{
				debugPrint("Error opening file %s\n",path);
				return false;
			}

			//Get the file length:
			int fseekres = fseek(file,0, SEEK_END);   //fseek==0 if ok
			long filelen = ftell(file);
			fseekres = fseek(file,0, SEEK_SET);

			//Read ethe ntire file into memory (!):
			if( addStringTermination )
				allocate(filelen+1);
			else
				allocate(filelen);

			size_t actualread = fread(_buf, _size, 1 ,file);  //must return 1
			
			if( addStringTermination ) 
				_buf[filelen] = '\0';
				
			fclose(file);

			return true;
		}

		size_t findString ( const char* search ) const
		{ 
			return findString(search,_size-_cur);
		}

		size_t findString ( const char* search, size_t size ) const
		{
			char* buffer = _buf+_cur;
			char* buffer0 = _buf+_cur;
			
			size_t len = strlen(search);
			bool fnd = false;
			while (!fnd)
			{
				fnd = true;
				for (size_t i=0; i<len; i++)
				{
					if (buffer[i]!=search[i])
					{
						fnd = false;
						break;
					}
				}
				if (fnd) 
					return buffer - buffer0 + _cur;
				buffer = buffer + 1;
				if (buffer - buffer0 + len >= size) 
					return -1;
			}
			return -1;
		}

		/// Get char
		char		getc() 
		{ 
			if(_cur>=_size)
				return EOF;

			char c = _buf[_cur]; 
			_cur++; 
			return c; 
		}

		float 	getFloat( bool swapEndian = false )
		{
			if(_cur>=_size)
				return 0;
#define			ENDIAN_32(dw) ((dw>>24)&0x000000FF) | ((dw>>8)&0x0000FF00) | ((dw<<8)&0x00FF0000) | ((dw<<24)&0xFF000000)			
			unsigned int dw = *((unsigned int*)(&_buf[_cur]));
			dw = ENDIAN_32(dw);
			
			float f = *((float*)(&dw));
			_cur+=4;
			return f;
		}
		
		unsigned char		getUChar() 
		{ 
			if(_cur>=_size)
				return EOF;

			unsigned char * ubuf = (unsigned char*)(&_buf[_cur]);
			_cur++;
			return ubuf[0];
		}
				
		void	ungetc()
		{
			if(_cur)
				_cur--;
		}

		std::string getLine()
		{
			std::string str = "";
			readLine(str);
			return str;
		}
		
		/// Read a line in buffer
		size_t readLine( std::string & str )
		{
			char c = getc();

			str="";
			while( c != '\n' && c!= '\r')
			{
				if( c == EOF ) 
					return -1;
				str+=c;
				c = getc();
			}
			return _cur;
		}

		int	fillStringArray( std::vector <std::string> & strings, char term )
		{
			bool res = true;
			
			while( res )
			{
				std::string str;
				if( readStringTillChar(str,term) == -1 )
					res = false;
				else
				{
					strings.push_back(str);
				}
			}

			return strings.size();
		}

		size_t readLineTillChar( std::string & str, char term )
		{
			str="";

			char c = getc();

			if(c==EOF)
				return -1;

			while( c != term && c!='\n' && c!='\r' )
			{
				if(c==EOF)
					return -1;
				str+=c;
				c = getc();
			}

			return _cur;
		}
		
		size_t readStringTillChar(  std::string & str, char term )
		{
			str="";

			char c = getc();

			if(c==EOF)
				return -1;

			while( c != term )
			{
				if(c==EOF)
					return -1;
				str+=c;
				c = getc();
			}

			return _cur;
		}

		size_t getToken(  std::string & str, char term=' ' )
		{
			skipWhitespace();
			
			str="";

			char c = getc();

			if(c==EOF)
				return -1;

			while( c != term && isprint(c) && c!=' ' ) //c!='\n' && c!=' ' && c!=term && c!='\t' )
			{
				if(c==EOF)
					return -1;
				str+=c;
				c = getc();
			}

			return _cur;
		}
		
		// test if a token is equal to testStr
		bool	testToken( const char * testStr, bool caseSensitive = true )
		{
			std::string test = testStr;
			std::string str;
			getToken(str);
			
			if(!caseSensitive)
			{
				toUpper( test );
				toUpper( str );
			}
			
			return str==test;			
		}
		
		/// skip whitespace
		size_t skipWhitespace()
		{
			char c = getc();
			while( !isalnum(c) && !ispunct(c))
			{
					if(c == EOF) 
						return -1;
					c = getc();
			}

			ungetc();
			
			return _cur;
		}

		
#define MAXDIGIT 20
		size_t readInt( int * num )
		{
			skipWhitespace();

			int i,c;
			char str[MAXDIGIT];

			i=0;

			while( isdigit(c = getc() )  && i < MAXDIGIT){
				str[i++] = c;
			}

			ungetc();

			str[i++] = '\0';
			i++;

			*num = atoi(str);

			return _cur;
		}
#undef MAXDIGIT

#define MAXDIGIT 40
		size_t readFloat( float * num)
		{
			// hacky shit
			char str[512];
			std::string s;
			getToken(s);
			strcpy( str,s.c_str() );
			
			sscanf(str,"%f",num);
			return _cur;
		}
#undef MAXDIGIT

		void	seek( size_t ind )
		{
			_cur = ind;
		}

		void	seekEnd()
		{
			_cur = _size-1;
		}

		void	seekStart()
		{
			_cur = 0;
		}

		void operator += ( size_t num )
		{
			if(_cur+num>=_size)
			{
				_cur = _size-1;
				return;
			}

			_cur += num;
		}

		void operator -= ( size_t num )
		{
			if(num>_cur)
			{
				_cur = 0;
				return;
			}

			_cur -= num;
		}
		
		operator const char * () const { return _buf+_cur; }

		size_t size() const { return _size; }
		char* get()  { return _buf+_cur; }
		const char * get() const { return _buf+_cur; }
		
		size_t current() const { return _cur; }
		bool	finished() { return _cur >= (_size-1); }
		
		char &operator [] (size_t index) const { return _buf[index+_cur]; }

		size_t _size;
		size_t _cur;
		char * _buf;
		
	protected:
		bool _bExternal;
		
	};

}
