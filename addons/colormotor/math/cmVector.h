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

#include "math/CMMathIncludes.h"
#include "math/CMVec2.h"
#include "math/CMVec3.h"
#include "math/CMVec4.h"
#include "math/CMMatrix4x4.h"
#include "math/CMMatrix3x3.h"
#include "math/CMQuat.h"

namespace cm
{

typedef std::pair<Vec2,Vec2> segment2;
typedef std::pair<Vec3,Vec3> segment3;
typedef std::pair<Vec4,Vec4> segment4;
typedef segment2 segment;


template<typename T,typename Y> inline TVec2<T> operator *( Y s, const TVec2<T> &v ) { return TVec2<T>( v.x * s, v.y * s ); }
template<typename T,typename Y> inline TVec2<T> operator *( const TVec2<T> &v, Y s ) { return TVec2<T>( v.x * s, v.y * s ); }
template<typename T,typename Y> inline TVec3<T> operator *( Y s, const TVec3<T> &v ) { return TVec3<T>( v.x * s, v.y * s, v.z * s ); }
template<typename T,typename Y> inline TVec3<T> operator *( const TVec3<T> &v, Y s ) { return TVec3<T>( v.x * s, v.y * s, v.z * s ); }
template<typename T,typename Y> inline TVec4<T> operator *( Y s, const TVec4<T> &v ) { return TVec4<T>( v.x * s, v.y * s, v.z * s, v.w * s ); }
template<typename T,typename Y> inline TVec4<T> operator *( const TVec4<T> &v, Y s ) { return TVec4<T>( v.x * s, v.y * s, v.z * s, v.w * s ); }

//// These functions are left overs from the DX version which used row major matrices.
/// Multiply a TVector by a 4x4 matrix
template <typename T>	
TVec2<T>	mul( const TVec2<T> & v, const TMatrix4x4<T> & m, bool translate = true );

/// Multiply a TVector by a 3x3 matrix
template <typename T> 
TVec2<T>  mul( const TVec2<T> & v, const TMatrix3x3<T> & m, bool translate = true );

/// Multiply a TVector by a 4x4 matrix
/// The translate flag indicates if translation is going to be applied ( homougeneous coordinates )
template <typename T>	
void	mul( TVec3<T> * out, const TVec3<T> & v, const TMatrix4x4<T> & m, bool translate = true );

/// Multiply a TVector by a 4x4 matrix
template <typename T>	
TVec3<T>	mul( const TVec3<T> & v, const TMatrix4x4<T> & m, bool translate = true );

/// Multiply a TVector by a 4x4 matrix
template <typename T>	
TVec4<T>	mul( const TVec4<T> & v, const TMatrix4x4<T> & m );


/// Multiply a TVector by a 4x4 matrix
template <typename T>	
TVec2<T>	mul( const TMatrix4x4<T> & m, const TVec2<T> & v, bool translate = true );

/// Multiply a TVector by a 3x3 matrix
template <typename T> 
TVec2<T>  mul( const TMatrix3x3<T> & m, const TVec2<T> & v, bool translate = true );

/// Multiply a TVector by a 4x4 matrix
/// The translate flag indicates if translation is going to be applied ( homougeneous coordinates )
template <typename T>	
void	mul( TVec3<T> * out, const TMatrix4x4<T> & m, const TVec3<T> & v, bool translate = true );

/// Multiply a TVector by a 4x4 matrix
template <typename T>	
TVec3<T>	mul( const TMatrix4x4<T> & m, const TVec3<T> & v, bool translate = true );

/// Multiply a TVector by a 4x4 matrix
template <typename T>	
TVec4<T>	mul( const TMatrix4x4<T> & m, const TVec4<T> & v );

	
	
template <typename T>
TVec4<T>	mul( const TVec4<T> & a, const TVec4<T> & b );
template <typename T>
TVec3<T>	mul( const TVec3<T> & a, const TVec3<T> & b );
template <typename T>
TVec2<T>	mul( const TVec2<T> & a, const TVec2<T> & b );

template <typename T>
TVec4<T>	div( const TVec4<T> & a, const TVec4<T> & b );
template <typename T>
TVec3<T>	div( const TVec3<T> & a, const TVec3<T> & b );
template <typename T>
TVec2<T>	div( const TVec2<T> & a, const TVec2<T> & b );
	
template <typename T>
TVec4<T>	div( const TVec4<T> & a, T v );
template <typename T>
TVec3<T>	div( const TVec3<T> & a, T v );
template <typename T>
TVec2<T>	div( const TVec2<T> & a, T v );
	
template <typename T>
TVec4<T>	mul( const TVec4<T> & a, T v );
template <typename T>
TVec3<T>	mul( const TVec3<T> & a, T v );
template <typename T>
TVec2<T>	mul( const TVec2<T> & a, T v );
	
template <typename T>
TVec4<T>	add( const TVec4<T> & a, const TVec4<T> & b );
template <typename T>
TVec3<T>	add( const TVec3<T> & a, const TVec3<T> & b );
template <typename T>
TVec2<T>	add( const TVec2<T> & a, const TVec2<T> & b );

template <typename T>
TVec4<T>	sub( const TVec4<T> & a, const TVec4<T> & b );
template <typename T>
TVec3<T>	sub( const TVec3<T> & a, const TVec3<T> & b );
template <typename T>
TVec2<T>	sub( const TVec2<T> & a, const TVec2<T> & b );

/// Project a 3d TVector with a projection matrix
template <typename T>	
TVec2<T>	project( const TVec3<T> & v, const TMatrix4x4<T> & m , int w, int h );
template <typename T>	
TVec3<T>	projectW( const TVec3<T> & v, const TMatrix4x4<T> & m, int w, int h );

/// Cross product
template <typename T>	
TVec2<T>	cross(const TVec2<T> & a,const TVec2<T> & b);
/// Cross product
template <typename T>	
void	cross(TVec2<T> * out, const TVec2<T> & a,const TVec2<T> & b);
/// Cross product
template <typename T>	
TVec3<T>	cross(const TVec3<T> & a,const TVec3<T> & b);
/// Cross product
template <typename T>	
void	cross(TVec3<T> * out, const TVec3<T> & a,const TVec3<T> & b);

/// Multiply and add
template <typename T>	
TVec2<T>	madd(const TVec2<T> & a,const TVec2<T> & b, const TVec2<T> & c);
/// Multiply and add
template <typename T>	
void	madd(TVec2<T> * out, const TVec2<T> & a,const TVec2<T> & b, const TVec2<T> & c);
/// Multiply and add
template <typename T>	
TVec3<T>	madd(const TVec3<T> & a,const TVec3<T> & b,const TVec3<T> & c);
/// Multiply and add
template <typename T>	
void	madd(TVec3<T> * out, const TVec3<T> & a,const TVec3<T> & b, const TVec3<T> & c);
/// Multiply and add
template <typename T>	
TVec4<T>	madd(const TVec4<T> & a,const TVec4<T> & b ,const TVec4<T> & c);
/// Multiply and add
template <typename T>	
void	madd(TVec4<T> * out, const TVec4<T> & a,const TVec4<T> & b, const TVec4<T> & c);

/// Dot product
template <typename T>	
T	dot(const TVec2<T> & a,const TVec2<T> & b);
/// Dot product
template <typename T>	
T	dot(const TVec3<T> & a,const TVec3<T> & b);
/// Dot product
template <typename T>	
T	dot(const TVec4<T> & a, const TVec3<T> & b);
/// Dot product
template <typename T>	
T	dot(const TVec3<T> & a, const TVec4<T> & b);
/// Dot product
template <typename T>	
T	dot(const TVec4<T> & a, const TVec4<T> & b);

/// Angle between two TVectors ( CMDIANS )
template <typename T>	
T	angleBetween( const TVec2<T> & a, const TVec2<T> & b );
/// Angle between two TVectors  ( CMDIANS )
template <typename T>	
T	angleBetween( const TVec3<T> & a, const TVec3<T> & b );

/// TVector length
template <typename T>	
T	length( const TVec2<T> & v );
/// TVector length
template <typename T>	
T	length( const TVec3<T> & v );
/// TVector length
template <typename T>	
T	length( const TVec4<T> & v );

/// Get normalized TVector
template <typename T>	
TVec2<T>	normalize( const TVec2<T> & v );
/// Get normalized TVector
template <typename T>	
TVec3<T>	normalize( const TVec3<T> & v );
/// Get normalized TVector
template <typename T>	
TVec4<T>	normalize( const TVec4<T> & v );

/// Distance between two floats (abs)
CM_INLINE float distance(float a, float b);
/// Distance between two points
template <typename T>	
T	distance(const TVec2<T> & a,const TVec2<T> & b);
/// Distance between two points
template <typename T>	
T	distance(const TVec3<T> & a,const TVec3<T> & b);
/// Square distance between two points
template <typename T>	
T	squareDistance(const TVec2<T> & a,const TVec2<T> & b);
/// Square distance between two points
template <typename T>	
T	squareDistance(const TVec3<T> & a,const TVec3<T> & b);

/// Linear interpolation 
template <typename T>	
TVec2<T>	lerp(const T & a,const T & b, T t);
/// Linear interpolation 
template <typename T>	
void	lerp(T * out, const T & a,const T & b, T t);

/// Clamp TVector
template <typename T>	
TVec2<T>	clamp(const TVec2<T> & a,const TVec2<T> & lo, const TVec2<T> & hi);
/// Clamp TVector
template <typename T>	
void	clamp(TVec2<T> * out, const TVec2<T> & lo,  const TVec2<T> & hi);
/// Clamp TVector
template <typename T>	
TVec3<T>	clamp(const TVec3<T> & a,const TVec3<T> & lo, const TVec3<T> & hi);
/// Clamp TVector
template <typename T>	
void	clamp(TVec3<T> * out, const TVec3<T> & lo,  const TVec3<T> & hi);
/// Clamp TVector
template <typename T>	
TVec4<T>	clamp(const TVec4<T> & a,const TVec4<T> & lo, const TVec4<T> & hi);
/// Clamp TVector
template <typename T>	
void	clamp(TVec4<T> * out, const TVec4<T> & lo,  const TVec4<T> & hi);


/// Clamp TVector to 0-1 range
template <typename T>	
TVec2<T>	clamp01(const TVec2<T> & a);
/// Clamp TVector to 0-1 range
template <typename T>	
void	clamp01(TVec2<T> * out);
/// Clamp TVector to 0-1 range
template <typename T>	
TVec3<T>	clamp01(const TVec3<T> & a);
/// Clamp TVector to 0-1 range
template <typename T>	
void	clamp01(TVec3<T> * out);
/// Clamp TVector to 0-1 range
template <typename T>	
TVec4<T>	clamp01(const TVec4<T> & a);
/// Clamp TVector to 0-1 range
template <typename T>	
void	clamp01(TVec4<T> * out);


/// TVector component min
template <typename T>	
TVec2<T>	min(const TVec2<T> & a,const TVec2<T> & b);
/// TVector component min
template <typename T>	
void	min(TVec2<T> * out, const TVec2<T> & a,const TVec2<T> & b);
/// TVector component min
template <typename T>	
TVec3<T>	min(const TVec3<T> & a,const TVec3<T> & b);
/// TVector component min
template <typename T>	
void	min(TVec3<T> * out, const TVec3<T> & a,  const TVec3<T> & b);
/// TVector component min
template <typename T>	
TVec4<T>	min(const TVec4<T> & a,const TVec4<T> & b);
/// TVector component min
template <typename T>	
void	min(TVec4<T> * out, const TVec4<T> & a,  const TVec4<T> & b);


/// TVector component max
template <typename T>	
TVec2<T>	max(const TVec2<T> & a,const TVec2<T> & b);
/// TVector component max
template <typename T>	
void	max(TVec2<T> * out, const TVec2<T> & a,const TVec2<T> & b);
/// TVector component max
template <typename T>	
TVec3<T>	max(const TVec3<T> & a,const TVec3<T> & b);
/// TVector component max
template <typename T>	
void	max(TVec3<T> * out, const TVec3<T> & a,  const TVec3<T> & b);
/// TVector component max
template <typename T>	
TVec4<T>	max(const TVec4<T> & a,const TVec4<T> & b);
/// TVector component max
template <typename T>	
void	max(TVec4<T> * out, const TVec4<T> & a,  const TVec4<T> & b);

template <typename T>	
T	maxComponent( const TVec2<T> & v );
template <typename T>	
T   maxComponent( const TVec3<T> & v );
//T maxComponent( const TVec4<T> & v );

template <typename T>	
T	minComponent( const TVec2<T> & v );
template <typename T>	
T   minComponent( const TVec3<T> & v );
//T minComponent( const TVec4<T> & v );

template <typename T>	
T triangleArea( const TVec2<T> & a, const TVec2<T> & b,const TVec2<T> & c );

//T			distanceToSegment( const TVec2<T> & p, const TVec2<T> & a, const TVec2<T> & b );
//T			squareDistanceToLine( const TVec2<T> & p, const TVec2<T> & a, const TVec2<T> & b );

///////////
/// IMPLEMENTATION

template <typename T>	
TVec2<T>	mul( const TVec2<T> & v, const TMatrix4x4<T>  & m, bool translate )
{
	Vec2 out;
	T t = (T)translate;
	out.x = m.m11 * v.x   +   m.m12 * v.y +	m.m14*t;
	out.y = m.m21 * v.x   +   m.m22 * v.y + m.m24*t;	
	return out;
}

template <typename T> 
TVec2<T> mul( const TVec2<T> & v, const TMatrix3x3<T>  & m, bool translate )
{
    Vec2 out;
    T t = (T)translate;
    out.x = m.m11 * v.x   +   m.m12 * v.y + m.m13*t;
    out.y = m.m21 * v.x   +   m.m22 * v.y + m.m23*t;    
    return out;
}
  


template <typename T>	
void	mul( TVec3<T> * out, const TVec3<T> & v, const TMatrix4x4<T> & m, bool translate )
{
	T t = (T)translate;
	out->x = m.m11 * v.x   +   m.m12 * v.y		+	m.m13 * v.z +	m.m14*t;
	out->y = m.m21 * v.x   +   m.m22 * v.y		+	m.m23 * v.z +   m.m24*t;	
	out->z = m.m31 * v.x   +   m.m32 * v.y		+	m.m33 * v.z +	m.m34*t;
}

template <typename T>
TVec3<T>	mul( const TVec3<T> & v, const TMatrix4x4<T> & m, bool translate )
{
	TVec3<T> out;
	T t = (T)translate;
	out.x = m.m11 * v.x   +   m.m12 * v.y		+	m.m13 * v.z +	m.m14*t;
	out.y = m.m21 * v.x   +   m.m22 * v.y		+	m.m23 * v.z +   m.m24*t;	
	out.z = m.m31 * v.x   +   m.m32 * v.y		+	m.m33 * v.z +	m.m34*t;
	return out;
}

template <typename T>
TVec4<T>	mul( const TVec4<T> & v, const TMatrix4x4<T> & m )
{
	TVec4<T> out;
	out.x = m.m11 * v.x   +   m.m12 * v.y		+	m.m13 * v.z +	m.m14 * v.w;
	out.y = m.m21 * v.x   +   m.m22 * v.y		+	m.m23 * v.z +   m.m24 * v.w;	
	out.z = m.m31 * v.x   +   m.m32 * v.y		+	m.m33 * v.z +	m.m34 * v.w;
	out.w = m.m41 * v.x   +   m.m42 * v.y		+	m.m43 * v.z +	m.m44 * v.w;
	return out;
}





template <typename T>	
TVec2<T>	mul( const TMatrix4x4<T>  & m, const TVec2<T> & v, bool translate )
{
	Vec2 out;
	T t = (T)translate;
	out.x = m.m11 * v.x   +   m.m12 * v.y +	m.m14*t;
	out.y = m.m21 * v.x   +   m.m22 * v.y + m.m24*t;	
	return out;
}

template <typename T> 
TVec2<T> mul( const TMatrix3x3<T>  & m, const TVec2<T> & v, bool translate )
{
    Vec2 out;
    T t = (T)translate;
    out.x = m.m11 * v.x   +   m.m12 * v.y + m.m13*t;
    out.y = m.m21 * v.x   +   m.m22 * v.y + m.m23*t;    
    return out;
}
  


template <typename T>	
void	mul( TVec3<T> * out, const TMatrix4x4<T> & m, const TVec3<T> & v, bool translate )
{
	T t = (T)translate;
	out->x = m.m11 * v.x   +   m.m12 * v.y		+	m.m13 * v.z +	m.m14*t;
	out->y = m.m21 * v.x   +   m.m22 * v.y		+	m.m23 * v.z +   m.m24*t;	
	out->z = m.m31 * v.x   +   m.m32 * v.y		+	m.m33 * v.z +	m.m34*t;
}

template <typename T>
TVec3<T>	mul( const TMatrix4x4<T> & m, const TVec3<T> & v, bool translate )
{
	TVec3<T> out;
	T t = (T)translate;
	out.x = m.m11 * v.x   +   m.m12 * v.y		+	m.m13 * v.z +	m.m14*t;
	out.y = m.m21 * v.x   +   m.m22 * v.y		+	m.m23 * v.z +   m.m24*t;	
	out.z = m.m31 * v.x   +   m.m32 * v.y		+	m.m33 * v.z +	m.m34*t;
	return out;
}

template <typename T>
TVec4<T>	mul( const TMatrix4x4<T> & m, const TVec4<T> & v )
{
	TVec4<T> out;
	out.x = m.m11 * v.x   +   m.m12 * v.y		+	m.m13 * v.z +	m.m14 * v.w;
	out.y = m.m21 * v.x   +   m.m22 * v.y		+	m.m23 * v.z +   m.m24 * v.w;	
	out.z = m.m31 * v.x   +   m.m32 * v.y		+	m.m33 * v.z +	m.m34 * v.w;
	out.w = m.m41 * v.x   +   m.m42 * v.y		+	m.m43 * v.z +	m.m44 * v.w;
	return out;
}







template <typename T>
TVec4<T>	mul( const TVec4<T> & a, const TVec4<T> & b )
{
	return TVec4<T>(a.x*b.x,
					a.y*b.y,
					a.z*b.z,
					a.w*b.w );
}
	
template <typename T>
TVec3<T>	mul( const TVec3<T> & a, const TVec3<T> & b )
{
	return TVec3<T>(a.x*b.x,
					a.y*b.y,
					a.z*b.z );
}

template <typename T>
TVec2<T>	mul( const TVec2<T> & a, const TVec2<T> & b )
{
	return TVec2<T>(a.x*b.x,
					a.y*b.y );
}

	
template <typename T>
TVec4<T>	div( const TVec4<T> & a, const TVec4<T> & b )
{
	return TVec4<T>(a.x/b.x,
					a.y/b.y,
					a.z/b.z,
					a.w/b.w );
}

template <typename T>
TVec3<T>	div( const TVec3<T> & a, const TVec3<T> & b )
{
	return TVec3<T>(a.x/b.x,
					a.y/b.y,
					a.z/b.z );
}

template <typename T>
TVec2<T>	div( const TVec2<T> & a, const TVec2<T> & b )
{
	return TVec2<T>(a.x/b.x,
					a.y/b.y );
}

template <typename T>
TVec4<T>	div( const TVec4<T> & a, T v )
{
	return TVec4<T>(a.x/v,
					a.y/v,
					a.z/v,
					a.w/v );
}

template <typename T>
TVec3<T>	div( const TVec3<T> & a, T v )
{
	return TVec3<T>(a.x/v,
					a.y/v,
					a.z/v );
}

template <typename T>
TVec2<T>	div( const TVec2<T> & a, T v )
{
	return TVec2<T>(a.x/v,
					a.y/v );
}

template <typename T>
TVec4<T>	mul( const TVec4<T> & a, T v )
{
	return TVec4<T>(a.x*v,
					a.y*v,
					a.z*v,
					a.w*v );
}

template <typename T>
TVec3<T>	mul( const TVec3<T> & a, T v )
{
	return TVec3<T>(a.x*v,
					a.y*v,
					a.z*v );
}

template <typename T>
TVec2<T>	mul( const TVec2<T> & a, T v )
{
	return TVec2<T>(a.x*v,
					a.y*v );
}


template <typename T>
TVec4<T>	add( const TVec4<T> & a, const TVec4<T> & b )
{
	return TVec4<T>(a.x+b.x,
					a.y+b.y,
					a.z+b.z,
					a.w+b.w );
}

template <typename T>
TVec3<T>	add( const TVec3<T> & a, const TVec3<T> & b )
{
	return TVec3<T>(a.x+b.x,
					a.y+b.y,
					a.z+b.z );
}

template <typename T>
TVec2<T>	add( const TVec2<T> & a, const TVec2<T> & b )
{
	return TVec2<T>(a.x+b.x,
					a.y+b.y );
}

	
template <typename T>
TVec4<T>	sub( const TVec4<T> & a, const TVec4<T> & b )
{
	return TVec4<T>(a.x-b.x,
					a.y-b.y,
					a.z-b.z,
					a.w-b.w );
}

template <typename T>
TVec3<T>	sub( const TVec3<T> & a, const TVec3<T> & b )
{
	return TVec3<T>(a.x-b.x,
					a.y-b.y,
					a.z-b.z );
}

template <typename T>
TVec2<T>	sub( const TVec2<T> & a, const TVec2<T> & b )
{
	return TVec2<T>(a.x-b.x,
					a.y-b.y );
}

template <typename T>
TVec2<T>	project( const TVec3<T> & v, const TMatrix4x4<T> & m, int w, int h )
{
	TVec4<T> pv = mul(TVec4<T>(v,1),m);;
	
	//TVec3<T>transform(&pv,pm,TVec);
	T div = 1.0f / pv.w;
	
	TVec2<T> out;
	out.x = ((pv.x * div + 1.0f)*0.5f) * w;
	out.y = h - ((pv.y * div + 1.0f)*0.5f) * h;
	
	return out;
}

template <typename T>
TVec3<T>	projectW( const TVec3<T> & v, const TMatrix4x4<T> & m, int w, int h )
{
	TVec4<T> pv = mul(TVec4<T>(v,1),m);;
	
	//TVec3<T>transform(&pv,pm,TVec);
	T div = 1.0f / pv.w;
	
	TVec3<T> out;
	out.x = ((pv.x * div + 1.0f)*0.5f) * w;
	out.y = h - ((pv.y * div + 1.0f)*0.5f) * h;
	out.z = pv.w;
	return out;
}


template <typename T>
TVec2<T>	cross(const TVec2<T> & a,const TVec2<T> & b)
{
	TVec2<T> out;
	out.x = a.x * b.y - a.y * b.x;
	out.y = a.y * b.x - a.x * b.y;
	return out;
}

template <typename T>
void	cross(TVec2<T> * out, const TVec2<T> & a,const TVec2<T> & b)
{
	out->x = a.x * b.y - a.y * b.x;
	out->y = a.y * b.x - a.x * b.y;
}

template <typename T>
TVec3<T>	cross(const TVec3<T> & a,const TVec3<T> & b)
{
	TVec3<T> out;
	
	out.x = a.y * b.z - a.z * b.y;
	out.y = a.z * b.x - a.x * b.z;
	out.z = a.x * b.y - a.y * b.x;
	
	return out;
	
}

template <typename T>
void	cross(TVec3<T> * out, const TVec3<T> & a,const TVec3<T> & b)
{
	out->x = a.y * b.z - a.z * b.y;
	out->y = a.z * b.x - a.x * b.z;
	out->z = a.x * b.y - a.y * b.x;
}


template <typename T>
TVec2<T>	madd(const TVec2<T> & a,const TVec2<T> & b, const TVec2<T> & c)
{
	return a*b+c;
}

template <typename T>
void	madd(TVec2<T> * out, const TVec2<T> & a,const TVec2<T> & b, const TVec2<T> & c)
{
	*out = a*b+c;
}

template <typename T>
TVec3<T>	madd(const TVec3<T> & a,const TVec3<T> & b, const TVec3<T> & c)
{
	return a*b+c;
}

template <typename T>
void	madd(TVec3<T> * out, const TVec3<T> & a,const TVec3<T> & b, const TVec3<T> & c)
{
	*out = a*b+c;
}

template <typename T>
TVec4<T>	madd(const TVec4<T> & a,const TVec4<T> & b, const TVec4<T> & c)
{
	return a*b+c;
}

template <typename T>
void	madd(TVec4<T> * out, const TVec4<T> & a,const TVec4<T> & b, const TVec4<T> & c)
{
	*out = a*b+c;
}



template <typename T>
T	dot(const TVec2<T> & a,const TVec2<T> & b)
{
	return (a.x * b.x + a.y * b.y);
}

template <typename T>
T	dot(const TVec3<T> & a,const TVec3<T> & b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

template <typename T>
T	dot(const TVec4<T> & a, const TVec3<T> & b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

template <typename T>
T	dot(const TVec3<T> & a, const TVec4<T> & b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

template <typename T>
T	dot(const TVec4<T> & a, const TVec4<T> & b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

template <typename T>
T	length( const TVec2<T> & v )
{
	return sqrtf (v.x * v.x + v.y * v.y );
}

template <typename T>
T	length( const TVec3<T> & v )
{
	return sqrtf (v.x * v.x + v.y * v.y + v.z * v.z );
}

template <typename T>
T	length( const TVec4<T> & v )
{
	return sqrtf (v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}


template <typename T>
TVec2<T>	normalize( const TVec2<T> & v )
{
	
	return v/sqrtf (v.x * v.x + v.y * v.y );
}


template <typename T>
TVec3<T>	normalize( const TVec3<T> & v )
{
	return v/sqrtf (v.x * v.x + v.y * v.y + v.z * v.z );
}

template <typename T>
TVec4<T>	normalize( const TVec4<T> & v )
{
	return v/sqrtf (v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

CM_INLINE float distance(float a, float b)
{
    return fabs(b-a);
}
    
template <typename T>
T	distance(const TVec2<T> & a,const TVec2<T> & b)
{
	TVec2<T> v = a-b;
	return sqrtf( dot(v,v) );
}

template <typename T>
T	distance(const TVec3<T> & a,const TVec3<T> & b)
{
	TVec3<T> v = a-b;
	return sqrtf( dot(v,v) );
}

template <typename T>
T	squareDistance(const TVec2<T> & a,const TVec2<T> & b)
{
	TVec2<T> v = a-b;
	return  dot(v,v);
}

template <typename T>
T	squareDistance(const TVec3<T> & a,const TVec3<T> & b)
{
	TVec3<T> v = a-b;
	return dot(v,v);
}

template <typename T>
TVec2<T>	clamp(const TVec2<T> & a,const TVec2<T> & lo, const TVec2<T> & hi)
{
	TVec2<T> out;
	out.x = clamp(a.x,lo.x,hi.x);
	out.y = clamp(a.y,lo.y,hi.y);
	return out;
}

template <typename T>
void	clamp(TVec2<T> * out, const TVec2<T> & lo, const TVec2<T> & hi)
{
	out->x = clamp(out->x,lo.x,hi.x);
	out->y = clamp(out->y,lo.y,hi.y);
}

template <typename T>
TVec3<T>	clamp(const TVec3<T> & a,const TVec3<T> & lo, const TVec3<T> & hi)
{
	TVec3<T> out;
	out.x = clamp(a.x,lo.x,hi.x);
	out.y = clamp(a.y,lo.y,hi.y);
	out.z = clamp(a.z,lo.z,hi.z);
	return out;
}

template <typename T>
void	clamp(TVec3<T> * out, const TVec3<T> & lo, const TVec3<T> & hi)
{
	out->x = clamp(out->x,lo.x,hi.x);
	out->y = clamp(out->y,lo.y,hi.y);
	out->z = clamp(out->z,lo.z,hi.z);
}


template <typename T>
TVec4<T>	clamp(const TVec4<T> & a,const TVec4<T> & lo, const TVec4<T> & hi)
{
	TVec4<T> out;
	out.x = clamp(a.x,lo.x,hi.x);
	out.y = clamp(a.y,lo.y,hi.y);
	out.z = clamp(a.z,lo.z,hi.z);
	out.w = clamp(a.w,lo.w,hi.w);
	return out;
}

template <typename T>
void	clamp(TVec4<T> * out, const TVec4<T> & lo, const TVec4<T> & hi)
{
	out->x = clamp(out->x,lo.x,hi.x);
	out->y = clamp(out->y,lo.y,hi.y);
	out->z = clamp(out->z,lo.z,hi.z);
	out->w = clamp(out->w,lo.w,hi.w);
}

template <typename T>
TVec2<T>	clamp01(const TVec2<T> & a)
{
	TVec2<T> out;
	out.x = clamp01(a.x);
	out.y = clamp01(a.y);
	return out;
}

template <typename T>
void	clamp01(TVec2<T> * out)
{
	out->x = clamp01(out->x);
	out->y = clamp01(out->y);
}

template <typename T>
TVec3<T>	clamp01(const TVec3<T> & a)
{
	TVec3<T> out;
	out.x = clamp01(a.x);
	out.y = clamp01(a.y);
	out.z = clamp01(a.z);
	return out;
}

template <typename T>
void	clamp01(TVec3<T> * out)
{
	out->x = clamp01(out->x);
	out->y = clamp01(out->y);
	out->z = clamp01(out->z);
}

template <typename T>
TVec4<T>	clamp01(const TVec4<T> & a)
{
	TVec4<T> out;
	out.x = clamp01(a.x);
	out.y = clamp01(a.y);
	out.z = clamp01(a.z);
	out.w= clamp01(a.w);
	return out;
}

template <typename T>
void	clamp01(TVec4<T>  * out)
{
	out->x = clamp01(out->x);
	out->y = clamp01(out->y);
	out->z = clamp01(out->z);
	out->w = clamp01(out->w);
}

template <typename T>
TVec2<T>	min(const TVec2<T> & a,const TVec2<T> & b)
{
	TVec2<T> out;
	out.x = (a.x<b.x)?a.x:b.x;
	out.y = (a.y<b.y)?a.y:b.y;
	return out;
}

template <typename T>
void	min(TVec2<T> * out, const TVec2<T> & a,const TVec2<T> & b)
{
	out->x = (a.x<b.x)?a.x:b.x;
	out->y = (a.y<b.y)?a.y:b.y;
}

template <typename T>
TVec3<T>	min(const TVec3<T> & a,const TVec3<T> & b)
{
	TVec3<T> out;
	out.x = (a.x<b.x)?a.x:b.x;
	out.y = (a.y<b.y)?a.y:b.y;
	out.z = (a.z<b.z)?a.z:b.z;
	return out;
}

template <typename T>
void	min(TVec3<T> * out, const TVec3<T> & a,  const TVec3<T> & b)
{
	out->x = (a.x<b.x)?a.x:b.x;
	out->y = (a.y<b.y)?a.y:b.y;
	out->z = (a.z<b.z)?a.z:b.z;
}

template <typename T>
TVec4<T>	min(const TVec4<T> & a,const TVec4<T> & b)
{
	TVec4<T> out;
	out.x = (a.x<b.x)?a.x:b.x;
	out.y = (a.y<b.y)?a.y:b.y;
	out.z = (a.z<b.z)?a.z:b.z;
	out.w = (a.w<b.w)?a.w:b.w;
	return out;
}

template <typename T>
void	min(TVec4<T> * out, const TVec4<T> & a,  const TVec4<T> & b)
{
	out->x = (a.x<b.x)?a.x:b.x;
	out->y = (a.y<b.y)?a.y:b.y;
	out->z = (a.z<b.z)?a.z:b.z;
	out->w = (a.w<b.w)?a.w:b.w;
}


template <typename T>
TVec2<T>	max(const TVec2<T> & a,const TVec2<T> & b)
{
	TVec2<T> out;
	out.x = (a.x>b.x)?a.x:b.x;
	out.y = (a.y>b.y)?a.y:b.y;
	return out;
}

template <typename T>
void	max(TVec2<T> * out, const TVec2<T> & a,const TVec2<T> & b)
{
	out->x = (a.x>b.x)?a.x:b.x;
	out->y = (a.y>b.y)?a.y:b.y;
}

template <typename T>
TVec3<T>	max(const TVec3<T> & a,const TVec3<T> & b)
{
	TVec3<T> out;
	out.x = (a.x>b.x)?a.x:b.x;
	out.y = (a.y>b.y)?a.y:b.y;
	out.z = (a.z>b.z)?a.z:b.z;
	return out;
}

template <typename T>
void	max(TVec3<T> * out, const TVec3<T> & a,  const TVec3<T> & b)
{
	out->x = (a.x>b.x)?a.x:b.x;
	out->y = (a.y>b.y)?a.y:b.y;
	out->z = (a.z>b.z)?a.z:b.z;
}

template <typename T>
TVec4<T>	max(const TVec4<T> & a,const TVec4<T> & b)
{
	TVec4<T> out;
	out.x = (a.x>b.x)?a.x:b.x;
	out.y = (a.y>b.y)?a.y:b.y;
	out.z = (a.z>b.z)?a.z:b.z;
	out.w = (a.w>b.w)?a.w:b.w;
	return out;
}

template <typename T>
void	max(TVec4<T> * out, const TVec4<T> & a,  const TVec4<T> & b)
{
	out->x = (a.x>b.x)?a.x:b.x;
	out->y = (a.y>b.y)?a.y:b.y;
	out->z = (a.z>b.z)?a.z:b.z;
	out->w = (a.w>b.w)?a.w:b.w;
}

// NEGATIVE CCW POSITIVE CW
template <typename T>
T angleBetween( const TVec2<T> & a, const TVec2<T> & b )
{
	// simplification of use of perp dot product for finding signed angle
	// TODO: check also heading and order coherence
	return atan2( a.x*b.y - a.y*b.x, a.x*b.x + a.y*b.y );
}

template <typename T>
T angleBetween( const TVec3<T> & a, const TVec3<T> & b )
{
	T lq=a.length()*b.length();
	
	if(lq<0.0001f)lq=0.0001f;	// to prevent division by zero
	
	T f=(dot(a,b))/lq;
	
	if(f>1.0f)f=1.0f;			// acos need input in the range [-1..1]
	else if(f<-1.0f)f=-1.0f;							//
	
	T res=(T)acos(f);	// cosf is not avaiable on every plattform
	
	return res;
}


template <typename T>
T	maxComponent( const TVec3<T> & v )
{
	if(v.x >= v.y)
		if(v.x >= v.z) 
			return v.x;
		else //v.z > v.x
			return v.z;
		else //v.y > v.x
			if(v.y >= v.z) 
				return v.y;
			else
				return v.z;
	
	return 0.0f;
}

template <typename T>
T maxComponent( const TVec2<T> & v )
{
	if(v.x >= v.y)
		return v.x;
	else
		return v.y;
	
	return 0.0f;
}

template <typename T>
T minComponent( const  TVec3<T> & v)
{
	if(v.x <= v.y)
		if(v.x <= v.z) 
			return v.x;
		else //v.z > v.x
			return v.z;
		else //v.y > v.x
			if(v.y <= v.z) 
				return v.y;
			else
				return v.z;
	
	return 0.0f;
}

template <typename T>
T minComponent(const  TVec2<T> & v)
{
	if(v.x <= v.y)
		return v.x;
	else
		return v.y;
	
	return 0.0f;
}

template <typename T>
T triangleArea( const TVec2<T> & a, const TVec2<T> & b,const TVec2<T> & c )
{
	TVec2<T> da = a-b;
	TVec2<T> db = c-b;
	T o = da.x * db.y - da.y * db.x;
	return o*0.5;
}

template <typename T>
void TVec3<T>::rotateX( T v )
{
	TMatrix4x4<T> m;
	m.rotationX(v);
	x = m.m11 * x   +   m.m12 * y		+	m.m13 * z;
	y = m.m21 * x   +   m.m22 * y		+	m.m23 * z;	
	z = m.m31 * x   +   m.m32 * y		+	m.m33 * z;
}

template <typename T>
void TVec3<T>::rotateY( T v )
{
	TMatrix4x4<T> m;
	m.rotationY(v);
	x = m.m11 * x   +   m.m12 * y		+	m.m13 * z;
	y = m.m21 * x   +   m.m22 * y		+	m.m23 * z;	
	z = m.m31 * x   +   m.m32 * y		+	m.m33 * z;
}

template <typename T>
void TVec3<T>::rotateZ( T v )
{
	TMatrix4x4<T> m;
	m.rotationZ(v);
	x = m.m11 * x   +   m.m12 * y		+	m.m13 * z;
	y = m.m21 * x   +   m.m22 * y		+	m.m23 * z;	
	z = m.m31 * x   +   m.m32 * y		+	m.m33 * z;
}	


// Quaternion:

template <typename T> 
TQuat<T>::TQuat( const TMatrix4x4<T> & m )
{
	fromMatrix4x4(m);
}

template <typename T> 
void TQuat<T>::fromMatrix4x4( const TMatrix4x4<T> & m )
{
	static const int plus1mod3[] = {1, 2, 0};
	
	// Find the index of the largest diagonal component
	// These ? operations hopefully compile to conditional
	// move instructions instead of branches.
	int i = (m.m22 > m.m11) ? 1 : 0;
	i = (m.m33 > m(i,i)) ? 2 : i;
	
	// Find the indices of the other elements
	int j = plus1mod3[i];
	int k = plus1mod3[j];
	
	// Index the elements of the TVector part of the TQuaternion as a T*
	// T* v = (T*)(this);
	
	// If we attempted to pre-normalize and trusted the matrix to be
	// perfectly orthonormal, the result would be:
	//
	//   double c = sqrt((m.m[i][i] - (m.m[j][j] + m.m[k][k])) + 1.0)
	//   v[i] = -c * 0.5
	//   v[j] = -(m.m[i][j] + m.m[j][i]) * 0.5 / c
	//   v[k] = -(m.m[i][k] + m.m[k][i]) * 0.5 / c
	//   w    =  (m.m[j][k] - m.m[k][j]) * 0.5 / c
	//
	// Since we're going to pay the sqrt anyway, we perform a post normalization, which also
	// fixes any poorly normalized input.  Multiply all elements by 2*c in the above, giving:
	
	// nc2 = -c^2
	
	T nc2 = ((m(j,j) + m(k,k)) - m(i,i)) - 1.0;
	vals[i] =  nc2;
	vals[3] =  (m(j,k) - m(k,j));
	vals[j] = -(m(i,j) + m(j,i));
	vals[k] = -(m(i,k) + m(k,i));
	/*
	 double nc2 = ((m.m[j][j] + m.m[k][k]) - m.m[i][i]) - 1.0;
	 vals[i] =  nc2;
	 vals[3] =  (m.m[j][k] - m.m[k][j]);
	 vals[j] = -(m.m[i][j] + m.m[j][i]);
	 vals[k] = -(m.m[i][k] + m.m[k][i]);
	 */
	// We now have the correct result with the wrong magnitude, so normalize it:
	T s = magnitude();
	if (s > 0.00001f) {
		s = 1.0f / s;
		x *= s;
		y *= s;
		z *= s;
		w *= s;
	} else {
		// The TQuaternion is nearly zero.  Make it 0 0 0 1
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f;
	}
}

template <typename T> 
void TQuat<T>::getMatrix4x4( TMatrix4x4<T> * out ) const
{
	TQuat<T> q = getUnit();
	
	T xx = 2.0f * q.x * q.x;
	T xy = 2.0f * q.x * q.y;
	T xz = 2.0f * q.x * q.z;
	T xw = 2.0f * q.x * q.w;
	
	T yy = 2.0f * q.y * q.y;
	T yz = 2.0f * q.y * q.z;
	T yw = 2.0f * q.y * q.w;
	
	T zz = 2.0f * q.z * q.z;
	T zw = 2.0f * q.z * q.w;
	
	T rot[] = { 1.0f - yy - zz,		xy + zw,		xz - yw,
		xy - zw,			1.0f - xx - zz, yz + xw,
		xz + yw,			yz - xw,		1.0f - xx - yy };
		
	out->identity();
	out->setRotation(rot);
}


// TMatrix4x4 cross defs
//////////////

template <typename T> TMatrix4x4<T>::TMatrix4x4( const TQuat<T> & q )
{
	identity();
	q.getMatrix4x4(this);
}

template <typename T> TMatrix4x4<T>::TMatrix4x4( const TQuat<T> & rot, const TVec3<T> & pos, const TVec3<T> & scale )
{
	fromQuaternion(rot,pos,scale);
}

template <typename T> 
const TMatrix4x4<T> & TMatrix4x4<T>::fromQuaternion( const TQuat<T> & rot, const TVec3<T> & pos, const TVec3<T> & scale )
{
	identity();
	rot.getMatrix4x4(this);
	m11 *= scale.x;
	m22 *= scale.y;
	m33 *= scale.z;
	m14 = pos.x;
	m24 = pos.y;
	m34 = pos.z;
	return *this;
}
	
	
template <typename T>
TVec2<T> & TVec2<T>::transform( const TMatrix4x4<T> & m, bool translate )
{
	T t = (T)translate;
	TVec2<T> v = *this;
	x = m.m11 * v.x   +   m.m12 * v.y +	m.m14*t;
	y = m.m21 * v.x   +   m.m22 * v.y + m.m24*t;	
	return *this;
}

	
template <typename T>
TVec3<T> & TVec3<T>::transform( const TMatrix4x4<T> & m, bool translate )
{
	T t = (T)translate;
	TVec3<T> v = *this;
	x = m.m11 * v.x   +   m.m12 * v.y		+	m.m13 * v.z +	m.m14*t;
	y = m.m21 * v.x   +   m.m22 * v.y		+	m.m23 * v.z +   m.m24*t;	
	z = m.m31 * v.x   +   m.m32 * v.y		+	m.m33 * v.z +	m.m34*t;
	return *this;
}

	
template <typename T>
TVec4<T> & TVec4<T>::transform( const TMatrix4x4<T> & m )
{
	TVec4<T> v = *this;
	x = m.m11 * v.x   +   m.m12 * v.y		+	m.m13 * v.z +	m.m14 * v.w;
	y = m.m21 * v.x   +   m.m22 * v.y		+	m.m23 * v.z +   m.m24 * v.w;	
	z = m.m31 * v.x   +   m.m32 * v.y		+	m.m33 * v.z +	m.m34 * v.w;
	w = m.m41 * v.x   +   m.m42 * v.y		+	m.m43 * v.z +	m.m44 * v.w;
	return *this;
}
	
	


}
