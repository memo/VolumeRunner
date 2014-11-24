#pragma once

#include "math/CMVector.h"

namespace cm {
	
	/// TPlane
	template  <typename T>
	struct  TPlane
	{
		
		TPlane()
		{
			
		}
		
		TPlane(T a, T b, T c, T d) 
		{
			eq(a,b,c,d);
		}
		
		TPlane( const TVec3<T> & n, T d)
		{
			eq(n,d);
		}
		
		void fromNormalAndPosition( const TVec3<T> & n, const TVec3<T> & pos )
		{
			eq.x = n.x;
			eq.y = n.y;
			eq.z = n.z;
			eq.w = dot(n,pos);
		}
		
		// \todo test..
		TPlane( const TVec3<T> & a, const TVec3<T> & b, const TVec3<T> & c )
		{
			eq = cross( a - b , c - b );
			eq.normalize();
			eq.w = -dot(eq,b);
		}
		
		TPlane( const TVec3<T> & n, const TVec3<T> & pos )
		{
			T d = -dot(pos,n);
			set(n,d);
		}
		
		const TVec3<T> & getNormal() const { return eq.xyz(); }
		
		void set(T a, T b, T c, T d) 
		{
			eq(a,b,c,d);
		}
		
		void set( const TVec3<T> & n, T d)
		{
			eq(n,d);
		}
		
		// \todo test..
		void set( const TVec3<T> & a, const TVec3<T> & b, const TVec3<T> & c )
		{
			eq = cross( a - b , c - b );
			eq.normalize();
			eq.w = -dot(eq,b);
		}
		
		void set( const TVec3<T> & n, const TVec3<T> & pos )
		{
			T d = -dot(pos,n);
			set(n,d);
		}
		
		
		
		T distance(const TVec3<T> & v)
		{
			return dot(Vec4(v,1),eq);
			//return (v.x * a + v.y * b + v.z * c )+d;
		}
		
		void	normalize()
		{
			eq.normalize();
		}
		
		/// returns closest point on TPlane
		TVec3<T> pointOnPlane(const TVec3<T> &p)
		{
			TVec3<T> n = eq.xyz();
			T d = dot(n, p-(n*(eq.w)));
			return p - (n * d);
		}
		
		bool	isPointInFront( const TVec3<T> & p )
		{
			if( distance(p) <= 0.0f )
				return true;
			return false;
		}
		
		TVec4<T>	eq; /// TPlane equation
		
	};
	
	typedef TPlane<float> Plane;
	typedef TPlane<float> Planef;
	typedef TPlane<double> Planed;
	
	/// @}
	
}