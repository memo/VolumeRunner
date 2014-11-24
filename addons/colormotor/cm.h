///////////////////////////////////////////////////////////////////////////                                                     
//	 _________  __   ____  ___  __  _______  __________  ___			
//	/ ___/ __ \/ /  / __ \/ _ \/  |/  / __ \/_  __/ __ \/ _ \			
// / /__/ /_/ / /__/ /_/ / , _/ /|_/ / /_/ / / / / /_/ / , _/			
// \___/\____/____/\____/_/|_/_/  /_/\____/ /_/  \____/_/|_|alpha.		
//																		
//  Daniel Berio 2008-2011												
//	http://www.enist.org/												
//																	
//																		
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#pragma once
 
#include "CMConfig.h"

#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>  //for ostringsream
#include <iomanip>  //for setprecision
#include <ext/hash_map>
#include <errno.h>

#if (defined (_WIN32) || defined (_WIN64))
#define      CM_WIN32 1
#define		  CM_WINDOWS 1
#elif defined(__APPLE_CPP__) || defined(__APPLE_CC__)
#include <CoreFoundation/CoreFoundation.h> // (needed to find out what platform we're using)
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#define     CM_IPHONE 1
#define     CM_IOS 1
#else
#define     CM_MAC 1
#endif
#else
#error "Unknown platform!"
#endif


#ifdef CM_WIN32

#include <windows.h>
#pragma warning(disable : 4244) // conversion from 'int' to 'float' possible loss of data
#pragma warning(disable : 4267) // conversion from 'size_t' to 'int', possible loss of data
#pragma warning(disable : 4305) //'argument' : truncation from 'double' to 'float'
#pragma warning(disable : 4018) // signed/unsigned mismatch
#pragma warning(disable : 4996) // deprecated fopen etc
#define CM_INLINE	__inline

#else

#define CM_INLINE	inline
#define CALLBACK

#endif



#define SAFE_DELETE( val )	if(val) { delete val; val=0; }
#define SAFE_DELETE_ARRAY( val ) if(val) { delete [] val; val = 0; }

#define CM_TEST( expr )		if( !(expr) ) return false
#define TEST_TRUE( expr )	if( !(expr) ) return false

#define DELETE_VECTOR( vec ) for( int i = 0; i < vec.size(); i++ ) if(vec[i]) delete vec[i]; vec.clear()

#define STRINGIFY( expr ) #expr

// always good to have PI around
#ifndef PI
#define PI				3.14159265358979323846
#endif

#ifdef __GNUC__
#define CM_GCC 1
#elif defined (_MSC_VER)
#define CM_MSVC 1
#else
#error unknown compiler
#endif



#if CM_DEBUG
// Assertions..
#if CM_WINDOWS

#if CM_GCC
#define breakDebugger			asm("int $3");
#else
#define breakDebugger			{ __asm int 3 }
#endif

#elif CM_MAC
	#if __MAC_OS_X_VERSION_MAX_ALLOWED >= 1050
		#define breakDebugger			Debugger();
	#else
		#define breakDebugger			asm("int $3");
	#endif
#elif CM_IOS
	#define breakDebugger			kill (0, SIGTRAP);
#endif

#define cmssertfalse			{ breakDebugger; }

#define cmssert(expression)     { if (! (expression)) cmssertfalse; }

#else

#define cmssertfalse			{ }
#define cmssert(a)				{ }


#endif




/// Got this code from fcollada...050: 
#define NOMINMAX /**< Rid us of the default versions of MINMAX. */
#ifdef max
#undef max
#endif // max
#ifdef min
#undef min
#endif // min

#define CMMINMAX

namespace cm
{

template<class T> inline T& tmax(T& x, T& y) { return (x > y) ? x : y; }
template<class T> inline const T& tmax(const T& x, const T& y) { return (x > y) ? x : y; }

template <class T> inline T& tmin(T& x, T& y) { return (x < y) ? x : y; }
template <class T> inline const T& tmin(const T& x,const T& y) { return (x < y) ? x : y; } 

inline float max( float a, float b ) { return tmax(a,b); }
inline float min( float a, float b ) { return tmin(a,b); } 
inline int max( int a, int b ) { return tmax(a,b); }
inline int min( int a, int b ) { return tmin(a,b); } 

typedef signed char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef unsigned char byte;

}

