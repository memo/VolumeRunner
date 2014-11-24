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

#include <math.h>
#include "CM.h"

namespace cm
{


// http://www.youtube.com/watch?v=eDiSYp_51iY&feature=related
#ifndef PI
#define PI				3.14159265358979323846
#endif

#define TWOPI           6.283185307179586476925287
#define PID2            1.570796326794896619231322
#define dia80PI			57.29577950560105
#define PIDIV180		0.0174532925199433

#define SQRTOFTWOINV 	0.707107
#define SQRTOFTWO		1.414213562373095

#define PHI				1.618033988749895
#define PHISQUARED		2.618033988749895
#define PHIINV			0.618033988749895

#ifndef ONETHIRD
#define ONETHIRD 0.333333333333333333333333333333333333333333333333333333333333
#endif

#define EPSILON 4.37114e-05

/// approx 1.0/255.0
#define DIV255			0.00392156862745098

//#define UNDEFINED		INFINITY

CM_INLINE double radians( double x ) { return PIDIV180*(x); }
CM_INLINE double degrees( double x ) { return x * dia80PI; }

CM_INLINE float sqr( float x ) { return x*x; }

/// Random float between 0 and 1
#ifdef CM_WINDOWS
CM_INLINE float	frand() { return (float)(rand())/RAND_MAX; }
#else
CM_INLINE float	frand() { return drand48(); }
#endif

/// Random float between a and b
CM_INLINE float frand( float a, float b ) { return a+frand()*(b-a); }

/// Random double between 0 and 1
CM_INLINE double drand() { return (double)(rand())/RAND_MAX; }

/// Random double between a and b
CM_INLINE double drand( double a, double b ) { return a+drand()*(b-a); }

/// random number returns a >= a && a < b
CM_INLINE int random( int a, int b ) { return a+(rand()%(b-a)); }

/// get rounded value
CM_INLINE float	roundf( float x) { return floorf((x) + 0.5f); }

/// percent true or false
CM_INLINE bool percent( float p )
{
	if( frand(0.0,100.0) >= (100.0-p) )
	{
		return true;
	}
	
	return false;
}
	
/// Linear interpolation
template <typename T,typename LT>  T	lerp( const T& a, const T& b, LT t )		{return a + (b - a)*t;}
/// Linear interpolation
CM_INLINE  float	lerp( float a, float b, float t )		{return a + (b - a)*t;}
/// Linear interpolation
CM_INLINE  double	lerp( double a, double b, double t )		{return a + (b - a)*t;}

/// Bilinear interpolation
/// between a and b (u), then b and c (u) and between results (v) 
template <class TT, class LT> TT bilerp(const TT& a, const TT& b, const TT& c, const TT& d, LT u, LT v)
{
	// TODO OPTIMIZE
	return (a+(b-a)*u) + ((c+(d-c)*u)-(a+(b-a)*u))*v;
}

/// s-curve function
template <typename T>
T	scurve( T t ) 	{return	( t * t * (3.0 - 2.0 * t)); }

/// return clamped value between lo and hi
template <typename T>
T	clamp( T in, T lo, T hi )
{
	if(in < lo) return lo; if(in > hi) return hi; return in;
}

/// return value clamped between 0 and 1	
template <typename T>
T	clamp01( T in )
{
	if(in < 0) return 0; if(in > 1) return 1; return in;
}
	
/// return value wrapped in a b range
template <typename T>
T	wrap( T val, T a, T b )
{
	T range = b-a;
	
	while( val > b )
		val-=range;
	while( val < a )
		val+=range;
		
	return val;
}

/// saw tooth function 
/// period one, returns value between 0 and 1
template <typename T>
T saw( T t )
{
	return t-floor(t);
}

/// traingle wave function 
/// period one, returns value between 0 and 1
template <typename T>
T triangle( T t )
{
	float ta = t*2.0;
	return fabs( 2.0*(ta-floor(ta+0.5)) );
}

/// Gaussian function
CM_INLINE
double gaussian(double n, double amt)
{
	return (double)((1.0 / sqrt(2.0 * PI * amt)) 
					* exp(-(n * n) / (2 * amt * amt)));
}
	
/// float equality with bias
template <typename T>
bool	fEqual( T a, T b, T eps = EPSILON )
{ return (fabs(b-a) < eps); }

/// modulo that wraps to last positive number ... so for example -1%5 = 4
template <class NTYPE> NTYPE mod( NTYPE a, NTYPE b )
{
	NTYPE n = (NTYPE)(a/b);
	a -= floor(n)*b;
	if( a<0 )
	{
		a += b;
	}
	return a;
}

/// TODO change my name
template <class NTYPE> NTYPE negMod( NTYPE val, NTYPE range )
{
	while( val > range )
		val -= range;
	while( val < 0 )
		val += range;
		
	return val;
}

/// Is value between other two.
template<class NTYPE> bool isBetween( NTYPE val, NTYPE a, NTYPE b )
{
	if( val >= a && val <= b )
		return true;
	if( val >= b && val <= a )
		return true;
	return false;
}
	
	
#ifndef CMMINMAX
template<class T> inline T& tmax(T& x, T& y) { return (x > y) ? x : y; }
template<class T> inline const T& tmax(const T& x, const T& y) { return (x > y) ? x : y; }

template <class T> inline T& tmin(T& x, T& y) { return (x < y) ? x : y; }
template <class T> inline const T& tmin(const T& x,const T& y) { return (x < y) ? x : y; } 

inline float max( float a, float b ) { return tmax(a,b); }
inline float min( float a, float b ) { return tmin(a,b); } 
inline int max( int a, int b ) { return tmax(a,b); }
inline int min( int a, int b ) { return tmin(a,b); } 
#endif



}