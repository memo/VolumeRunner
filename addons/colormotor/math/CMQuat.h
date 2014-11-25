#pragma once

#include "math/CMMathDefs.h"
#include "math/CMVec3.h"

namespace cm {
	
	template <typename T> struct TMatrix4x4;
	
	/// TQuaternion
	template <typename T>
	struct   TQuat
	{
		
		TQuat()
		{
			// should remove these
			x=0.0f; y=0.0f; z=0.0f; w=1.0f;
		}
		
		TQuat( const TQuat<T> & q )
		{
			x=q.x; y=q.y; z=q.z; w=q.w;
		}
		
		
		TQuat( T ix, T iy, T iz, T iw )
		{
			x=ix; y=iy; z=iz; w=iw;
		}
		
		/// From axis angle
		TQuat( const TVec3<T> & v , T rot )
		{
			axisAngle(v,rot);
		}
		
		// implementation from http://cache-www.intel.com/cd/00/00/29/37/293748_293748.pdf
		/// \todo this does not seem to be a very good implementation
		TQuat( const TMatrix4x4<T> & m );
		
		/// From euler angle ( x y z rotation )
		TQuat( T x, T y , T z )
		{
			euler(x,y,z);
		}
		
		/// From euler angle ( x y z rotation )
		TQuat( const Vec3 & v )
		{
			euler(v.x,v.y,v.z);
		}
		
		
		~TQuat()
		{
		}
		
        bool hasNans() const { if(isnan(x) || isnan(y) || isnan(z) || isnan(w)) return true; return false; }
		
		TQuat<T>& operator () (T x_, T y_, T z_, T w_) {
			x = x_; y = y_; z = z_; w = w_;
			return *this;
		}
		
		TQuat<T>& operator ()( const TVec3<T> & v , T rot )
		{
			axisAngle(v,rot);
			return *this;
		}
		
		TQuat<T>& operator ()(  T x, T y , T z )
		{
			euler(x,y,z);
			return *this;
		}
		
		TQuat<T>& operator ()(   const TVec3<T> & v )
		{
			euler(v);
			return *this;
		}
		
		
		// Implementation from Watt and Watt, pg 362
		// See also http://www.flipcode.com/documents/matrfaq.html#Q54
		void	getMatrix4x4(  TMatrix4x4<T> * out ) const;
		
		inline void euler( const TVec3<T> & v )
		{
			euler(v.x,v.y,v.z);
		}
		
		/// from matrix TQuaternion FAQ
		void euler( T x, T y, T z )
		{
			TQuat qx(Vec3(1,0,0),x);
			TQuat qy(Vec3(0,1,0),y);
			TQuat qz(Vec3(0,0,1),z);
			
			TQuat qt = qx*qy;
			*this = qt*qz;
		}
		
		
		inline void identity() { x=y=z=0.0f; w=1.0f; }
		
		void	set( T ix, T iy, T iz, T iw )
		{	x=ix; y=iy; z=iz; w=iw;	}	
		
		// in radians
		void setAngle(T f)
		{
			Vec3 v = getAxis();

			w = (T) cos(  f  / 2.0 );
			v *= sin(f/2.0);
			x = v.x;
			y = v.y;
			z = v.z;
		}

		void scaleAngle(T f)
		{
			setAngle( f * getAngle() );
		}

		// assumes w is between -1 and 1
		T	getAngle() const 
		{
			return 2.0*acos(w);
		}

		TVec3<T>	getAxis() const 
		{
			 T scale = (T) sin( acos( w ) );

			if ( scale < 0.00001 && scale > -0.00001 )
				return TVec3<T>( 0.0, 0.0, 0.0 );
			else
				return  TVec3<T>(x,y,z) / scale;
		}
		
		void	direction( const TVec3<T> & dir )
		{
			//assert(0); // This is broken ( flipped )
			x = dir.y;
			y = -dir.x;
			z = 0.0f;
			w = 1.0f + dir.z;
			
			if ( x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f ) {
				*this = TQuat<T>( 0, 1, 0, 0 ); // If we can't normalize it, just set it
			} else {
				normalize();
                assert(!hasNans());
			}
		}
		
		/// set from axis and angle
		void	axisAngle( const TVec3<T> & v, T angle )
		{	
			T ang = angle * 0.5f;
			T sa = sin(ang); // /2?
			x = v.x * sa;
			y = v.y * sa;
			z = v.z * sa;
			w = cos(ang);
		}
		
		void fromMatrix4x4( const TMatrix4x4<T> & m );
		
		TQuat<T>  operator - () const
		{
			return TQuat(-x,-y,-z,-w);
		}
		
		
		inline TQuat<T> conjugate() const
		{
			return TQuat(-x,-y,-z,w);
		}
		
		inline TQuat<T> inverse() const {
			return conjugate() / ( x*x + y*y + z*z + w*w );
		}
		
		void operator += ( const TQuat<T>&q )
		{
			x += q.x; y += q.y; z+=q.z; w+=q.w;
		}
		
		void operator -= ( const TQuat<T>&q )
		{
			x -= q.x; y -= q.y; z-=q.z; w-=q.w;
		}
		
		void operator *= ( const TQuat<T>&b )
		{
			T tx=b.x,ty=b.y,tz=b.z,tw=b.w;
			x = tw*b.x + tx*b.w + ty*b.z - tz*b.y;
			y = tw*b.y - tx*b.z + ty*b.w + tz*b.x;
			z = tw*b.z + tx*b.y - ty*b.x + tz*b.w;
			w = tw*b.w - tx*b.x - ty*b.y - tz*b.z;
		}
		
		void operator *= ( T f)
		{
			x*=f; y*=f; z*=f; w*=f;
		}
		
		void operator /= ( T f)
		{
			x/=f; y/=f; z/=f; w/=f;
		}
		
		TQuat<T> operator + ( const TQuat<T>&q ) const	{ return TQuat<T>(x+q.x,y+q.y,z+q.z,w+q.w); }
		
		TQuat<T> operator - ( const TQuat<T>&q ) const	{ return TQuat<T>(x-q.x,y-q.y,z-q.z,w-q.w); }
		
		TQuat<T> operator * ( const TQuat<T>&b ) const
		{
			TQuat<T> out;
			out.x = w*b.x + x*b.w + y*b.z - z*b.y;
			out.y = w*b.y - x*b.z + y*b.w + z*b.x;
			out.z = w*b.z + x*b.y - y*b.x + z*b.w;
			out.w = w*b.w - x*b.x - y*b.y - z*b.z;
			//normalize();
			return out;
		}
		
		TVec3<T> operator * ( const TVec3<T> & v ) const
		{
			TVec3<T> uv, uuv;
			TVec3<T> qvec(x, y, z);
			uv = cross(qvec,v);
			uuv = cross(qvec,uv);
			uv *= (2.0f * w);
			uuv *= 2.0f;
			
			return v + uv + uuv;
		}
		
		TQuat<T> operator * ( T f) const
		{
			return TQuat<T>(x*f,y*f,z*f,w*f);
		}
		TQuat<T> operator / ( T f) const
		{
			return TQuat<T>(x/f,y/f,z/f,w/f);
		}
		
		
		bool operator == ( const TQuat<T>&q ) const
		{
			return ( fEqual(x,q.x) && fEqual(y,q.y) && fEqual(z,q.z) && fEqual(w,q.w));
		}
		bool operator != ( const TQuat<T>&q ) const
		{
			return ( !fEqual(x,q.x) && !fEqual(y,q.y) && !fEqual(z,q.z) && !fEqual(w,q.w));
		}
		
		T	magnitude() { return sqrt(x*x + y*y + z*z + w*w); }
		void	normalize() 
		{
			T len = x*x + y*y + z*z + w*w;
            len = sqrt(len);
			T rs = 1.0f/len;
			x*=rs;
			y*=rs;
			z*=rs;
			w*=rs;
            assert(!hasNans());
		}
		
		TQuat<T>	getUnit() const { TQuat<T> q=*this; q.normalize(); return q; }
		
		TVec3<T>	getXYZRotation() const 
		{
			TVec3<T> v(0,0,0);
			
			// yaw
			v.y = atan2(  	2 * y * w - 2 * x * z, 
                   		1 - 2 * SQR(y) - 2 * SQR(z) );
			
			// roll
			v.z = asin( 2 * x * y + 2 * z * w );
			
			// pitch
			v.x = atan2(	2 * x * w - 2 * y * z, 
						1 - 2 * SQR(x) - 2 * SQR(z) ); 
			
			
			if(x*y*z*w == 0.5) 
			{ 
				v.y = 2 * atan2(x,w); 
				v.x = 0; 
			} 
			else if(x*y*z*w == -0.5) 
			{ 
				v.y = -2 * atan2(x,w);  
				v.x = 0; 
			} 
			
			//yaw -> y
			//pitch -> x
			//roll -> z
			return v;
			
		}
		
		inline  T operator[] (int i) const {
			return ((T*)this)[i];
		}
		
		union
		{
			struct{
				T x;
				T y;
				T z;
				T w;
			};
			
			T vals[4];
		};
		
		
	};
	
	typedef TQuat<float> Quat;
	typedef TQuat<float> Quatf;
	typedef TQuat<double> Quatd;
	
	/// TQuaternion dot product
	template <typename T> 
	T	dot(  const TQuat<T> & a, const TQuat<T> & b );
	
	
	/// Spherically interpolate between two TQuaternions
	template <typename T> 
	TQuat<T> slerp( const TQuat<T> & a, const TQuat<T> & b, T t, T minAngle );
	
	/// @}
	
	
	
	/// IMPLEMENTATION
	// See CMVector.h for cross definitions with Matrix4x4

	template <typename T> 
	T dot(  const TQuat<T> & a, const TQuat<T> & b )
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	template <typename T> 
	void lerp( TQuat<T> * out, const TQuat<T> & a, const TQuat<T> & b, double t )
	{
		out->x = lerp(a.x,b.x,t);
		out->y = lerp(a.y,b.y,t);
		out->z = lerp(a.z,b.z,t);
		out->w =  lerp(a.w,b.w,t);
	}

	template <typename T> 
	TQuat<T> slerp( const TQuat<T> & a, const TQuat<T> & b, double t, double minAngle = 0.001 )
	{
		if (t <= 0.0f)
			return  a;
		if (t >= 1.0f)
			return b;
		
		// angle between TQuaternion rotations
		T phi;
		T cosphi = dot(a,b);
		
		TQuat<T> qa = a;
		
		if (cosphi < 0) 
		{
			// Change the sign and fix the dot product; we need to
			// loop the other way to get the shortest path
			qa = -qa;
			cosphi = -cosphi;
		}
		
		if(cosphi < -1.0f)
			cosphi = -1.0f;
		if(cosphi > 1.0f)
			cosphi = 1.0f;
		
		phi = acos(cosphi);
		
		if (phi >= minAngle) 
		{
			T s0 = sin((1.0f - t) * phi);
			T s1 = sin(t * phi);
			
			return  ( (qa * s0) + (b * s1) ) / sin(phi);
		} 
		else 
		{
			// For small angles, linear interpolate
			return  lerp(qa,b,t);
		}
		
	}



}