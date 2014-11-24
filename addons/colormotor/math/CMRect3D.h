#pragma once 

#include "math/CMPlane.h"

namespace cm
{

	/// TRect3D
	template <typename T>
	struct TRect3D
	{
		TRect3D( const TVec3<T> & pos, const TVec3<T> & u,  const TVec3<T> & v, float w, float h ) :
		l(l),t(t),r(r),b(b),u(u),v(v)
		{
			TVec3<T> n = cross(u,v);
			plane.set(n,pos);
			u0 = dot(pos,u);
			v0 = dot(pos,v);
			u1 = u0+w;
			v1 = v0+h;
		}
		
		float getWidth() { return u1-u0; }
		float getHeight() { return v1-v0; }
		
		TVec3<T> getCenter()
		{
			// kinda expencive maybe precalc?
			TVec3<T> pos = plane.eq.xyz() * plane.eq.w;
			return pos + u * (( u0+u1 )*0.5) + v * (( v0+v1 )*0.5);
			return pos;
		}
		
		// offsets on plane of rect
		T u0,v0;
		T u1,v1;
		
		// TODO remove unions, they are annoying to debug
		union{
			T l;
			T left;
		};
		union{
			T t;
			T top;
		};
		union{
			T r;
			T right;
		};
		union{
			T b;
			T bottom;
		};
		
		
		TVec3<T>	u;
		TVec3<T>	v;
		TPlane<T>	plane;
		
	};
	
	typedef TRect3D<float> Rect3D;
	typedef TRect3D<float> Rect3Df;
	typedef TRect3D<double> Rect3Dd;
	
	/// @}
	
}