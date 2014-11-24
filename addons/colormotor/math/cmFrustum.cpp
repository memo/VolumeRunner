#include "cmFrustum.h"


namespace cm
{

void Frustum::update( const M44 & matrix ) 
{  
#ifdef D3D9 
	// Left clipping plane 
    frustum[LEFT].eq.x = matrix.m14 + matrix.m11; 
   	frustum[LEFT].eq.y = matrix.m24 + matrix.m21; 
    frustum[LEFT].eq.z = matrix.m34 + matrix.m31; 
    frustum[LEFT].eq.w = matrix.m44 + matrix.m41; 
    
	// Right clipping plane 
    frustum[RIGHT].eq.x = matrix.m14 - matrix.m11; 
    frustum[RIGHT].eq.y = matrix.m24 - matrix.m21; 
    frustum[RIGHT].eq.z = matrix.m34 - matrix.m31; 
    frustum[RIGHT].eq.w = matrix.m44 - matrix.m41;
	 
    // Top clipping plane 
    frustum[TOP].eq.x = matrix.m14 - matrix.m12; 
    frustum[TOP].eq.y = matrix.m24 - matrix.m22; 
    frustum[TOP].eq.z = matrix.m34 - matrix.m32; 
    frustum[TOP].eq.w = matrix.m44 - matrix.m42; 
	
    // Bottom clipping plane 
    frustum[BOTTOM].eq.x = matrix.m14 + matrix.m12; 
    frustum[BOTTOM].eq.y = matrix.m24 + matrix.m22; 
    frustum[BOTTOM].eq.z = matrix.m34 + matrix.m32; 
    frustum[BOTTOM].eq.w = matrix.m44 + matrix.m42; 
	
    // Near clipping plane 
    frustum[FRONT].eq.x = matrix.m13; 
    frustum[FRONT].eq.y = matrix.m23; 
    frustum[FRONT].eq.z = matrix.m33; 
    frustum[FRONT].eq.w = matrix.m43; 
	
    // Far clipping plane 
    frustum[BACK].eq.x = matrix.m14 - matrix.m13; 
    frustum[BACK].eq.y = matrix.m24 - matrix.m23; 
    frustum[BACK].eq.z = matrix.m34 - matrix.m33; 
    frustum[BACK].eq.w = matrix.m44 - matrix.m43; 
#else
 
	// Left clipping plane 
    frustum[LEFT].eq.x = matrix.m41 + matrix.m11; 
   	frustum[LEFT].eq.y = matrix.m42 + matrix.m12; 
    frustum[LEFT].eq.z = matrix.m43 + matrix.m13; 
    frustum[LEFT].eq.w = matrix.m44 + matrix.m14; 
    
	// Right clipping plane 
    frustum[RIGHT].eq.x = matrix.m41 - matrix.m11; 
    frustum[RIGHT].eq.y = matrix.m42 - matrix.m12; 
    frustum[RIGHT].eq.z = matrix.m43 - matrix.m13; 
    frustum[RIGHT].eq.w = matrix.m44 - matrix.m14;
	 
    // Top clipping plane 
    frustum[TOP].eq.x = matrix.m41 - matrix.m21; 
    frustum[TOP].eq.y = matrix.m42 - matrix.m22; 
    frustum[TOP].eq.z = matrix.m43 - matrix.m23; 
    frustum[TOP].eq.w = matrix.m44 - matrix.m24; 
	
    // Bottom clipping plane 
    frustum[BOTTOM].eq.x = matrix.m41 + matrix.m21; 
    frustum[BOTTOM].eq.y = matrix.m42 + matrix.m22; 
    frustum[BOTTOM].eq.z = matrix.m43 + matrix.m23; 
    frustum[BOTTOM].eq.w = matrix.m44 + matrix.m24;  
	
	 // Near clipping plane 
    frustum[FRONT].eq.x = matrix.m41 + matrix.m31; 
    frustum[FRONT].eq.y = matrix.m42 + matrix.m32; 
    frustum[FRONT].eq.z = matrix.m43 + matrix.m33; 
    frustum[FRONT].eq.w = matrix.m44 + matrix.m34;
	
    // Far clipping plane 
    frustum[BACK].eq.x = matrix.m41 - matrix.m31; 
    frustum[BACK].eq.y = matrix.m42 - matrix.m32; 
    frustum[BACK].eq.z = matrix.m43 - matrix.m33; 
    frustum[BACK].eq.w = matrix.m44 - matrix.m34;

/*
// Left clipping plane 

    frustum[LEFT].eq.x = matrix.m14 + matrix.m11; 
   	frustum[LEFT].eq.y = matrix.m24 + matrix.m21; 
    frustum[LEFT].eq.z = matrix.m34 + matrix.m31; 
    frustum[LEFT].eq.w = matrix.m44 + matrix.m41; 
    
	// Right clipping plane 
    frustum[RIGHT].eq.x = matrix.m14 - matrix.m11; 
    frustum[RIGHT].eq.y = matrix.m24 - matrix.m21; 
    frustum[RIGHT].eq.z = matrix.m34 - matrix.m31; 
    frustum[RIGHT].eq.w = matrix.m44 - matrix.m41;
	 
    // Top clipping plane 
    frustum[TOP].eq.x = matrix.m14 - matrix.m12; 
    frustum[TOP].eq.y = matrix.m24 - matrix.m22; 
    frustum[TOP].eq.z = matrix.m34 - matrix.m32; 
    frustum[TOP].eq.w = matrix.m44 - matrix.m42; 
	
    // Bottom clipping plane 
    frustum[BOTTOM].eq.x = matrix.m14 + matrix.m12; 
    frustum[BOTTOM].eq.y = matrix.m24 + matrix.m22; 
    frustum[BOTTOM].eq.z = matrix.m34 + matrix.m32; 
    frustum[BOTTOM].eq.w = matrix.m44 + matrix.m42; 
	
	 // Near clipping plane 
    frustum[FRONT].eq.x = matrix.m14 + matrix.m13; 
    frustum[FRONT].eq.y = matrix.m24 + matrix.m23; 
    frustum[FRONT].eq.z = matrix.m34 + matrix.m33; 
    frustum[FRONT].eq.w = matrix.m44 + matrix.m43;
	
    // Far clipping plane 
    frustum[BACK].eq.x = matrix.m14 - matrix.m13; 
    frustum[BACK].eq.y = matrix.m24 - matrix.m23; 
    frustum[BACK].eq.z = matrix.m34 - matrix.m33; 
    frustum[BACK].eq.w = matrix.m44 - matrix.m43; 
*/

#endif


	// normalize
	for( int i = 0; i < 6; i++ )
	{
		//frustum[i].eq = -frustum[i].eq;
		frustum[i].normalize();
	}
	
}

///////////////////////////////////////////////

int Frustum::pointIn(const Vec3 &p) 
{
	int res = INSIDE;
	for(int i=0; i < 6; i++) {

		if (frustum[i].distance(p) < 0)
			return OUTSIDE;
	}
	return res;
}

///////////////////////////////////////////////


int Frustum::sphereIn( const Vec3 &p, float radius) 
{
	int res = INSIDE;
	float distance;
/*
	for(int i=0; i < 6; i++) {
		distance = frustum[i].distance(p);
		if (distance < -radius)
			return OUTSIDE;
		else if (distance < radius)
			res =  INTERSECT;
	}*/
	
	for(int i=0; i < 6; i++)
	{
		if (frustum[i].distance(p) + radius < 0)
			return OUTSIDE;
	}
		
	return res;
}

///////////////////////////////////////////////


int Frustum::aaboxIn( const AABox &box )
{
	int res = INSIDE;
	for(int i=0; i < 6; i++) {

		if (frustum[i].distance(getAABBVertexPos(box,frustum[i].getNormal())) < 0)
			return OUTSIDE;
		else if (frustum[i].distance(getAABBVertexNeg(box,frustum[i].getNormal())) < 0)
			res =  INTERSECT;
	}
	return res;
}

Vec3 Frustum::getAABBVertexPos( const AABox & box, const Vec3 & n )
{
	const Vec3 &vmax = box.getMax();
	Vec3 res = box.getMin();

	if (n.x > 0)
		res.x = vmax.x;

	if (n.y > 0)
		res.y = vmax.y;

	if (n.z > 0)
		res.z = vmax.z;

	return res;
}



Vec3 Frustum::getAABBVertexNeg( const AABox & box, const Vec3 & n )
{
	const Vec3 &vmax = box.getMax();
	Vec3 res = box.getMin();

	if (n.x < 0)
		res.x = vmax.x;

	if (n.y < 0)
		res.y = vmax.y;

	if (n.z < 0)
		res.z = vmax.z;

	return res;
}


}