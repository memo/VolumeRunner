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

#include "math/CMVector.h"


namespace cm {
	
	
	/// Transform class
	/// Handles a transformation with a quaternion rotation and position
	/// \todo scale?
	template <typename T>
	class  TTransform
	{
	public:
		
		TTransform() : position(0,0,0)
		{
			identity();
		}
		
		TTransform( const TVec3<T> & pos, const TQuat<T> & rot ) 
		: 
		rotation(rot),
		position(pos)
		{
		}
		
		TTransform& operator () (const TMatrix4x4<T> & m) {
			rotation.fromMatrix4x4(m);
			position = m.trans();
			return *this;
		}
		
		TTransform& operator () (const TVec3<T> & pos, const Quat & rot) {
			position = pos;
			rotation = rot;
			return *this;
		}
		
		TTransform( const TMatrix4x4<T> & m  )
		{
			rotation.fromMatrix4x4(m);
			position = m.trans();
		}
		
		void		preRotate( T x, T y, T z )
		{
			TQuat<T> q(x,y,z);
			rotation = q*rotation;
		}
		
		void		rotate( T x, T y, T z )
		{
			TQuat<T> q(x,y,z);
			rotation *= q;
		}
		
		
		void		pointAt( const TVec3<T> & v )
		{
			TVec3<T> dir = v - position;
			dir.normalize();
			rotation.direction(dir);
		}
		
		/// todo!!
		void		identity()
		{
			position(0,0,0);
			scale(1,1,1);
			rotation.identity();
		}
		
		/// convert to matrix
		/// slow
		const TMatrix4x4<T>	&m44()  { return tm(rotation,position,scale); }
		/// convert to matrix
		/// slow
		operator const TMatrix4x4<T>&()  { return m44(); }
		
		TQuat<T> rotation;
		TVec3<T> position;
		TVec3<T> scale;
		
	protected:
		TMatrix4x4<T> tm;
		
	};
	
	typedef TTransform<float> Transform;
	typedef TTransform<float> Transformf;
	typedef TTransform<double> Transformd;
	
	/// Spherical interpolation
	/// performs slerp on rotation component and lerp on position
	template <typename T>
	TTransform<T> slerp( const TTransform<T> & a, const TTransform<T> & b, double t, double minAngle = 0.001 )
	{
		TTransform<T> out;
		out.rotation = slerp(a.rotation,b.rotation,t,minAngle);
		out.position = lerp(a.position,b.position,t);
		out.scale = lerp(a.scale,b.scale,t);
		return out;
	}
	
	/// @}



}