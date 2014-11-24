#pragma once


#include "math/cmVector.h"
#include "math/cmAABox.h"
#include "math/cmPlane.h"

namespace cm
{
	//#define TESTME
	
	class Frustum  {
	public:
		enum
		{
			OUTSIDE = 0,
			INSIDE,
			INTERSECT
		};
		
		enum {
			LEFT	= 0,	
			RIGHT,		
			TOP,		
			BOTTOM,		
			FRONT,		
			BACK		
		}; 

		Frustum(){}
		virtual ~Frustum(){}

		void update(const M44 & viewProjection);

		int pointIn(const Vec3 &point);
		int sphereIn(const Vec3 &center,  float radius);
		int aaboxIn( const AABox &box );
		
		Plane	frustum[6];
	private:
		 Vec3 getAABBVertexPos( const AABox & box, const Vec3 & n );
		 Vec3 getAABBVertexNeg( const AABox & box, const Vec3 & n );
	
	};

}