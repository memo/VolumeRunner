#pragma once

#include "math/CMVector.h"
#include "math/CMPlane.h"
#include "math/CMMatrix4x4.h"
#include "math/CMRect3D.h"

namespace cm {
	
	/// \addtogroup Math
	/// @{
	
	template  <typename T>
	struct TRayIntersection
	{
		TVec3<T> pos;
		float u;
		float v;
		float t;
		bool result;
	};
	
	/// TRay Charles
	template  <typename T>
	struct   TRay
	{
		public :
		TVec3<T> origin;
		TVec3<T> direction;
		
		TRay() { origin.zero(); direction.zero(); }
		
		TRay(	const TVec3<T> &origin_,
			const TVec3<T> &direction_,
			bool normalize_ = false ) 
		{ 
			set(origin_,direction_,normalize_);
		}
		
		void	set(	const TVec3<T> &origin_,
					const TVec3<T> &direction_,
					bool normalize_ = false ) 
		{
			origin = origin_;
			direction = direction_;
			if(normalize_)
				direction.normalize();
		}
		
		void	fromPositions(	const TVec3<T> &a,
							  const TVec3<T> &b)
		{
			set(a,b-a,true);
		}
		
		void	normalize()
		{
			direction.normalize();
		}
		
		TRay<T> &	rotate( const TMatrix4x4<T> &m )
		{
			TVec3<T> tmp = direction;
			direction.x  = tmp.x*m._11 + tmp.y*m._21 + tmp.z*m._31;
			direction.y  = tmp.x*m._12 + tmp.y*m._22 + tmp.z*m._32;
			direction.z  = tmp.x*m._13 + tmp.y*m._23 + tmp.z*m._33;
			return *this;
		}
		
		/// TRay plane intesection
		/// \todo Triangle sphere etc...!!!!!
		bool	intersect( TRayIntersection<T> * out, const TPlane<T> & plane ,bool oneSided = true )
		{
			
			float b = dot(plane.eq,direction);
			
			// find if TRay is parallel
			if( b == 0)
			{
				return false;
			}
			
			// find if TRay is pointing away from plane
			if( oneSided && b > 0.0f )
			{
				return false;
			}
			
			float a = -(dot(plane.eq,origin) + plane.eq.w);
			
			out->t = a/b;
			
			//	debugPrint("TRay intersection: ");
			// calculate intersection point 
			out->pos = origin+direction*(out->t);
			out->result = true;
			return true;
		}
		
		/// TRay 3d rect intesection
		bool	intersect( TRayIntersection<T> * out, const TRect3D<T> & rect ,bool oneSided = true )
		{
			if( intersect(out,rect.plane,oneSided) == false )
				return false;
			
			// 
			out->u = dot( out->pos, rect.u );
			out->v = dot( out->pos, rect.v );
			
			if( isBetween( out->u, rect.u0, rect.u1 ) &&
			   isBetween( out->v, rect.v0, rect.v1 ) )
				return true;
			
			out->result = false;
			return false;
		}
		
		
		/// \todo REFLECT!
	};
	
	typedef TRayIntersection<float> RayIntersection;
	typedef TRay<float> Ray;

	typedef TRayIntersection<float> RayIntersectionf;
	typedef TRay<float> Rayf;

	typedef TRayIntersection<double> RayIntersectiond;
	typedef TRay<double> Rayd;

	/// @}
	
}