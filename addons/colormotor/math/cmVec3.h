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

#include "CMMathIncludes.h"
#include "cmVec2.h"

namespace cm
{

	template <typename T> struct TMatrix4x4;
	
	
/// 3D Vector
template <class T>
struct  TVec3
{
    TVec3(const ofVec3f & v)
    :
    x(v.x),
    y(v.y),
    z(v.z)
    {
        
    }
    
	TVec3(T x, T y, T z) 
	: 
	x(x),y(y),z(z)
	{
	}
	
	TVec3(T v)
	:
	x(v),y(v),z(v)
	{
	}


	TVec3() 
	{
		x = 0.0f; y = 0.0f; z = 0.0f;
	}
	
	TVec3( const TVec3<T> & v ) 
	{
		x = v.x; y = v.y; z = v.z; 
	}
	
	TVec3( const TVec2<T> & v )
	{
		x = v.x;
		y = v.y;
		z = 0;
	}
	
	TVec3( T ar[3] )
	{
		memcpy(this,ar,sizeof(TVec3<T>));
	}
	
	TVec3<T>& operator () (T x_, T y_, T z_) 
	{
		x = x_; y = y_; z = z_; 
		return *this;
	}
	
	TVec3<T>& operator () (const TVec3<T> & v ) {
		x = v.x; y = v.y; z = v.z; 
		return *this;
	}
	
    bool hasNans() const { if(isnan(x) || isnan(y) || isnan(z)) return true; return false; }
    
	void set(T x_, T y_, T z_) { x = x_; y = y_; z = z_; }
	
	void set( const TVec3<T> & v ) { x = v.x; y = v.y; z = v.z;  }
	
	static TVec3<T> getRandom( T a, T b )
	{
		TVec3<T> v;
		v.rand(a,b);
		return v;
	}
	
	void spherical( T radius, T theta, T phi )
	{
		// theta 0-TWOPI longitutde
		// phi = 0-PI latitude
		
		x = radius * cos(theta)*sin(phi);
		y = radius * sin(theta)*sin(phi);
		z = radius * cos(phi);
	}
	
	static TVec3<T> getUnitRandom()
	{
		TVec3<T> v;
		v.unitRand();
		return v;
	}
	
	int	getGreaterMagnitude()
	{
		T ax = fabs(x);
		T ay = fabs(y);
		T az = fabs(z);
		if( ax > ay )
		{
			if( ax > az ) 
				return 0;
			else
				return 2;
		}
		else
		{
			if( ay > az )
				return 1;
		}
		
		return 2;
	}
	
	int	getLeastMagnitude()
	{
		T ax = fabs(x);
		T ay = fabs(y);
		T az = fabs(z);
		if( ax < ay )
		{
			if( ax < az ) 
				return 0;
			else
				return 2;
		}
		else
		{
			if( ay < az )
				return 1;
		}
		
		return 2;
	}
	
	void	wrap( T a, T b )
	{
		T range = b-a;
		
		while( x > b )
			x-=range;
		while( x < a )
			x+=range;
			
		while( y > b )
			y-=range;
			while( y < a )
			y+=range;
			
		while( z > b )
			z-=range;
		while( z < a )
			z+=range;
			
	}
	
	void	floor() { x = floorf(x); y = floorf(y); z = floorf(z); }
	
	void	rand( T a, T b )
	{
		x = frand(a,b);
		y = frand(a,b);
		z = frand(a,b);
	}
	
	void	unitRand()
	{
		x = frand(-1,1);
		y = frand(-1,1);
		z = frand(-1,1);
		normalize();
	}
	
	T length() const { return (T) sqrt(x*x + y*y + z*z); }
	
	T squareLength() const { return  x*x + y*y + z*z ; }
	
	static TVec3<T> zero() { return TVec3<T>(0,0,0); }
		
	static const TVec3<T> & unitX() { static TVec3<T> v(1,0,0); return v; }
	static const TVec3<T> & unitY() { static TVec3<T> v(0,1,0); return v; }
	static const TVec3<T> & unitZ() { static TVec3<T> v(0,0,1); return v; }
	
	/// Swizzles
	TVec3<T> xxx() const { return TVec3<T>(x,x,x); }
	TVec3<T> yyy() const { return TVec3<T>(y,y,y); }
	TVec3<T> zzz() const { return TVec3<T>(y,y,y); }
	
	TVec3<T> zyx() const { return TVec3<T>(z,y,x); }
	TVec3<T> zxy() const { return TVec3<T>(z,x,y); }
	TVec3<T> yxz() const { return TVec3<T>(y,x,z); }
	TVec3<T> yzx() const { return TVec3<T>(y,z,x); }
	
	TVec2<T> xy() const { return TVec2<T>(x,y); }
	TVec2<T> xz() const { return TVec2<T>(x,z); }
	TVec2<T> yz() const { return TVec2<T>(y,z); }
	
	void rotateX( T v );
	void rotateY( T v );
	void rotateZ( T v );
	
	TVec3<T> normalized() const 
	{
		TVec3 <T> v = *this;
		v.normalize();
		return v;
	}
	
	void normalize() {
		T m = length();
		if (m > 0) {
			T inv = 1.0f/m;
			x*=inv;
			y*=inv;
			z*=inv;
		}
	}
	
	void limit(T max) {
		T m =  length();
		if (m > max) {
			T inv = 1.0f/m;
			inv*=max;
			x*=inv;
			y*=inv;
			z*=inv;
		}
	}
	
	T heading2D() const {
		T angle = (T) atan2(-y, x);
		return -1*angle;
	}
	
	T distance( const TVec3 <T> & v ) const
	{
		TVec3<T> d = *this - v;
		return sqrt( d.x*d.x+d.y*d.y+d.z*d.z );
	}

	T squareDistance( const TVec3 <T> & v ) const
	{
		TVec3<T> d = *this - v;
		return d.x*d.x+d.y*d.y+d.z*d.z;
	}
	
	// unary operators
	TVec3<T>  operator - () const
	{
		return TVec3(-x,-y,-z);
	}
	
	TVec3<T> & add( const TVec3<T> & v ) { x+=v.x; y+=v.y; z+=v.z; return *this; }
	TVec3<T> & sub( const TVec3<T> & v ) { x-=v.x; y-=v.y; z-=v.z; return *this; }
	TVec3<T> & mul( const TVec3<T> & v ) { x*=v.x; y*=v.y; z*=v.z; return *this; }
	TVec3<T> & div( const TVec3<T> & v ) { x/=v.x; y/=v.y; z/=v.z; return *this; }
	TVec3<T> & mul( float v ) { x*=v; y*=v; z*=v; return *this; }
	TVec3<T> & div( float v ) { x/=v; y/=v; z/=v; return *this; }
	TVec3<T> & negate() { x=-x; y=-y; z=-z; return *this; }
	TVec3<T> & transform( const TMatrix4x4<T> & m, bool translate = true );
	
	TVec3<T>& operator += ( const TVec3<T>& v)	{ x+=v.x; y+=v.y; z+=v.z; return *this; }
	TVec3<T>& operator -= ( const TVec3<T>& v)	{ x-=v.x; y-=v.y; z-=v.z;return *this; }
	TVec3<T>& operator *= ( T f)		{ x*=f; y*=f; z*=f;return *this;  }
	TVec3<T>& operator *= ( const TVec3<T>& v)	{ x*=v.x; y*=v.y; z*=v.z; return *this; }
	TVec3<T>& operator /= ( T f)		{ x/=f; y/=f; z/=f;return *this; }
	
	TVec3<T> operator + ( const TVec3<T>& v) const		{ return TVec3<T>(x+v.x, y+v.y, z+v.z); }
	TVec3<T> operator - ( const TVec3<T>& v ) const		{ return TVec3<T>(x-v.x, y-v.y, z-v.z); }
	TVec3<T> operator * ( T f) const			{ return TVec3<T>(x*f, y*f, z*f); }
	TVec3<T> operator * ( const TVec3<T>& v) const		{ return TVec3<T>(x*v.x, y*v.y, z*v.z); }
	TVec3 operator / ( T f) const			{ return TVec3(x/f, y/f, z/f); }

	bool equals( const TVec3<T>& v) const { return (fEqual(x,v.x) && fEqual(y,v.y) && fEqual(z,v.z)); }
	bool operator == ( const TVec3<T>& v) const { return (x==v.x && y==v.y && z==v.z); }
	bool operator != ( const TVec3<T>& v) const { return (x!=v.x || y!=v.y || z!=v.z); }
	bool operator < ( const TVec3<T>& v) const 
	{ 
		T d1 = (x*x+y*y+z*z);
		T d2 = (v.x*v.x+v.y*v.y+v.z*v.z);
		return d1<d2;
	}
	
	inline  T &operator[] (int i){
		return ((T*)this)[i];
	}
	
	inline  const T &operator[] (int i) const {
		return ((T*)this)[i];
	}
    
    operator const ofVec3f () const { return ofVec3f(x,y,z); }
	
	operator T* () { return (T*)this; }
	operator const T* () const { return (const T*)this; }

	unsigned int hash() const 
	{
		return ((unsigned int)((73856093*getBits(x)) ^ 
		(19349663*getBits(y)) ^ 
		(83492791*getBits(z)) ));
	}
	
	std::string toString()  const
	{
		std::string s;
		stdPrintf(s,"%.5f %.5f %.5f",x,y,z);
		return s;
	}
	
	T x,y,z;
};


typedef TVec3<float> float3;
typedef TVec3<double> double3;

typedef TVec3<float> Vec3f;
typedef TVec3<double> Vec3d;
typedef TVec3<int> Vec3i;
typedef Vec3f Vec3;


}