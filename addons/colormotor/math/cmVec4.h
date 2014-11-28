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

#include "CMMathIncludes.h"
#include "cmVec3.h"

namespace cm
{
	template <typename T> struct TMatrix4x4;
	
/// 4D Vector
template <typename T>
struct  TVec4
{
	TVec4(T x, T y, T z, T w) 
	:
	x(x),y(y),z(z),w(w)
	{
	}
	
	TVec4(T v)
	:
	x(v),y(v),z(v),w(v)
	{
	}
	
	TVec4() {
		x = 0.0f; y = 0.0f; z = 0.0f;
	}
	
	TVec4( const TVec4<T> & v ) {
		x = v.x; y = v.y; z = v.z; w = v.w;
	}
	
	TVec4( const TVec3<T> & v ) {
		x = v.x; y = v.y; z = v.z;  w = 0.0f;
	}
	
	TVec4( const TVec3<T> & v, T w_ ) {
		x = v.x; y = v.y; z = v.z; w = w_;
	}
	
	TVec4( T ar[4] )
	{
		memcpy(this,ar,sizeof(TVec4<T>));
	}
	
	TVec4<T>& operator () (T x_, T y_, T z_, T w_) 
	{
		x = x_; y = y_; z = z_; w = w_;
		return *this;
	}
	
	void set (T x_, T y_, T z_, T w_) 
	{
		x = x_; y = y_; z = z_; w = w_;
	}
	
	TVec4<T>& operator () ( const TVec3<T> & v, T w_ ) 
	{
		x = v.x; y = v.y; z = v.z; w = w_;
		return *this;
	}
	
	
	TVec4<T>& operator () (T x_, T y_, T z_) 
	{
		x = x_; y = y_; z = z_;
		return *this;
	}
	
	T length() const {
		return (T) sqrt(x*x + y*y + z*z + w*w);
	}
	
	T squareLength() const{
		return  x*x + y*y + z*z + w*w; // TOGLIERE w?
	}
	
	const TVec3<T> &xyz() const { return *((TVec3<T>*)this); }// TVec3(x,y,z); }
	
	TVec4<T> operator = (const TVec4<T> & v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}
	
	void normalize() {
		T m = length();
		if (m > 0) {
			T inv = 1.0f/m;
			x*=inv;
			y*=inv;
			z*=inv;
			w*=inv;
		}
	}
	
	// unary operators
	TVec4<T>  operator - () const
	{
		return TVec4<T>(-x,-y,-z,-w);
	}
	
	TVec4<T> & add( const TVec4<T> & v ) { x+=v.x; y+=v.y; z+=v.z;  w+=v.w; return *this; }
	TVec4<T> & sub( const TVec4<T> & v ) { x-=v.x; y-=v.y; z-=v.z;  w-=v.w; return *this; }
	TVec4<T> & mul( const TVec4<T> & v ) { x*=v.x; y*=v.y; z*=v.z;  w*=v.w; return *this; }
	TVec4<T> & div( const TVec4<T> & v ) { x/=v.x; y/=v.y; z/=v.z;  w/=v.w; return *this; }
	TVec4<T> & mul( float v ) { x*=v; y*=v; z*=v;  w*=v; return *this; }
	TVec4<T> & div( float v ) { x/=v; y/=v; z/=v;  w/=v; return *this; }
	TVec4<T> & negate() { x=-x; y=-y; z=-z; w=-w;  return *this; }
	TVec4<T> & transform( const TMatrix4x4<T> & m );
	
	TVec4<T>& operator += ( const TVec4<T>& v)	{ x+=v.x; y+=v.y; z+=v.z; w+=v.w;return *this;  }
	TVec4<T>& operator -= ( const TVec4<T>& v)	{ x-=v.x; y-=v.y; z-=v.z; w-=v.w;return *this;  }
	TVec4<T>& operator *= ( T f)		{ x*=f; y*=f; z*=f; w*=f;return *this;  }
	TVec4<T>& operator *= ( const TVec4<T>& v)	{ x*=v.x; y*=v.y; z*=v.z; w*=v.w;return *this;  }
	TVec4<T>& operator /= ( T f)		{ x/=f; y/=f; z/=f; w/=f;return *this;  }
	
	TVec4<T> operator + ( const TVec4<T>& v) const		{ return TVec4<T>(x+v.x, y+v.y, z+v.z, w+v.w); }
	TVec4<T> operator - ( const TVec4<T>& v ) const		{ return TVec4<T>(x-v.x, y-v.y, z-v.z, w-v.w); }
	TVec4<T> operator * ( T f) const			{ return TVec4<T>(x*f, y*f, z*f, w*f); }
	TVec4<T> operator * ( const TVec4<T>& v) const		{ return TVec4<T>(x*v.x, y*v.y, z*v.z, w*v.w); }
	TVec4<T> operator / ( T f) const			{ return TVec4<T>(x/f, y/f, z/f, w/f); }
	
	bool operator == ( const TVec4<T>& v) const { return (x==v.x && y==v.y && z==v.z && w==v.w); }
	bool operator != ( const TVec4<T>& v) const { return (x!=v.x || y!=v.y || z!=v.z || w!=v.w); }
	
	//bool operator < ( const TVec4<T>& v) const { return (x<v.x && y<v.y && z<v.z && w<v.w); }
	inline  T & operator[] (int i) {
		return ((T*)this)[i];
	}
	
	inline  const T & operator[]  (int i) const {
		return ((T*)this)[i];
	}
	
	operator T* () { return (T*)this; }
	operator const T* () const { return (const T*)this; }
	
	std::string toString()  const
	{
		std::string s;
		stdPrintf(s,"%.5f %.5f %.5f %.5f",x,y,z,w);
		return s;
	}
	
	
	T x,y,z,w;
};

typedef TVec4<float> float4;
typedef TVec4<double> double4;
typedef TVec4<float> Vec4f;
typedef TVec4<double> Vec4d;
typedef TVec4<int> Vec4i;
typedef Vec4f Vec4;

}
