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

#define CM_VEC2

namespace cm
{
	template <typename T> struct TMatrix4x4;
	
	/// 2D Vector
	template <typename T> 
	struct   TVec2{
	public:
		TVec2(T x, T y)
		:
		x(x),y(y)
		{
		}
		
		TVec2(T v)
		:
		x(v),y(v)
		{
		}
		
		TVec2() {
		}
		
		
		TVec2( const TVec2<T> & v ) {
			x = v.x; y = v.y; 
		}
		
		
		TVec2( T ar[2] )
		{
			memcpy(this,ar,sizeof(TVec2<T>));
		}
		
		TVec2& operator () (T x_, T y_){
			x = x_; y = y_;
			return *this;
		}
		
		void set( T x_, T y_ ) { x = x_; y = y_; }
		
		operator T* () { return (T*)this; }
		operator const T* () const { return (const T*)this; }
		
		T length() const {
			return (T) sqrt(x*x + y*y);
		}
		
		T squareLength() const {
			return (x*x + y*y);
		}
		
		void	rand( T a, T b )
		{
			x = frand(a,b);
			y = frand(a,b);
			//x = frandBetween(a,b);
		}
		
		void	unitRand()
		{
			x = frand(-1.0,1.0);
			y = frand(-1.0,1.0);
			//z = nfrand();
			normalize();
		}
		
		/// Swizzles
		TVec2<T> yx() const { return TVec2(y,x); }
		
		TVec2<T> normalized() const 
		{
			TVec2 <T> v = *this;
			v.normalize();
			return v;
		}
		
		void normalize() {
			T m = length();
			if (m > 0) {
				T inv = 1.0f/m;
				x*=inv;
				y*=inv;
				//z*=inv;
			}
		}
		
		void limit(T max) {
			T m =  length();
			if (m > max) {
				T inv = 1.0f/m;
				inv*=max;
				x*=inv;
				y*=inv;
				//z*=inv;
			}
		}
		
		TVec2<T> getPerp() const 
		{
			return TVec2(-y,x);
		}
		
		// Vector heading ( angle in radians )
		T heading() const {
			T angle = atan2(y, x);
			if (angle < 0) angle += TWOPI;
			return angle;
		}
		
		void rotate( T radians )
		{
			T cosa = cos( radians );
			T sina = sin( radians );
			T rx = x * cosa - y * sina;
			y = x * sina + y * cosa;
			x = rx;
		}
		
		// unary operators
		TVec2<T>  operator - () const
		{
			return TVec2(-x,-y);
		}
		
		T distance( const TVec2 <T> & v ) const
		{
			TVec2<T> d = *this - v;
			return sqrt( d.x*d.x+d.y*d.y );
		}

		T squareDistance( const TVec2 <T> & v ) const
		{
			TVec2<T> d = *this - v;
			return d.x*d.x+d.y*d.y;
		}

		unsigned int hash() const 
		{
			return ((unsigned int)((73856093*getBits(x)) ^ 
								   (19349663*getBits(y))  ));
		}
		
		bool equals(const TVec2<T> & v ) const
		{
			return (fEqual(x,v.x) && fEqual(y,v.y));
		}

		
		static TVec2<T> zero() { return TVec2<T>(0,0); }
		
		TVec2<T> & add( const TVec2<T> & v ) { x+=v.x; y+=v.y; return *this; }
		TVec2<T> & sub( const TVec2<T> & v ) { x-=v.x; y-=v.y; return *this; }
		TVec2<T> & mul( const TVec2<T> & v ) { x*=v.x; y*=v.y; return *this; }
		TVec2<T> & mul( float v ) { x*=v; y*=v; return *this; }
		TVec2<T> & div( float v ) { x/=v; y/=v; return *this; }
		TVec2<T> & negate() { x=-x; y=-y; return *this; }
		TVec2<T> & transform( const TMatrix4x4<T> & m, bool translate = true );
		
		TVec2<T>& operator += ( const TVec2<T>& v)	{ x+=v.x; y+=v.y; return *this; }
		TVec2<T>& operator -= ( const TVec2<T>& v)	{ x-=v.x; y-=v.y; return *this; }
		TVec2<T>& operator *= ( T f)		{ x*=f; y*=f; return *this;  }
		TVec2<T>& operator *= ( const TVec2<T>& v)	{ x*=v.x; y*=v.y; return *this; }
		TVec2<T>& operator /= ( T f)		{ x/=f; y/=f; return *this; }
		TVec2<T>& operator /= ( const TVec2<T>& v)	{ x/=v.x; y/=v.y; return *this; }
		
		TVec2<T> operator + ( const TVec2<T>& v) const		{ return TVec2<T>(x+v.x, y+v.y); }
		TVec2<T> operator - ( const TVec2<T>& v ) const		{ return TVec2<T>(x-v.x, y-v.y); }
		TVec2<T> operator * ( T f) const			{ return TVec2<T>(x*f, y*f); }
		TVec2<T> operator * ( const TVec2& v) const		{ return TVec2<T>(x*v.x, y*v.y); }
		TVec2<T> operator / ( T f) const			{ return TVec2<T>(x/f, y/f); }
		TVec2<T> operator / ( const TVec2& v) const		{ return TVec2<T>(x/v.x, y/v.y); }
		
		bool operator == ( const TVec2<T>& v) const { return (x==v.x && y==v.y ); }
		bool operator != ( const TVec2<T>& v) const { return (x!=v.x || y!=v.y ); }
		
		inline const T &operator[] (int i) const {
			return ((T*)this)[i];
		}
		
		inline  T &operator[] (int i)  {
			return ((T*)this)[i];
		}
			
		std::string toString() const 
		{
			std::string s;
			
			stdPrintf(s,"%.5f %.5f",x,y);
			return s;
		}
		
		T x;
		T y;
		
	};
	
	typedef TVec2<float> float2;
	typedef TVec2<double> double2;
	
	typedef TVec2<float> Vec2f;
	typedef TVec2<double> Vec2d;
	typedef TVec2<int> Vec2i;

	typedef Vec2f Vec2;
}
